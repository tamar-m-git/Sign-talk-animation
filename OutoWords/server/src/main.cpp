#include <crow.h>
#include "Outomation.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct CORS {
    struct context {};
    void before_handle(crow::request&, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
    void after_handle(crow::request&, crow::response& res, context&) {}
};

int main() {
    crow::App<CORS> app;
    Outomation automaton;
    automaton.LoadFromFile("C:/CodesProject/TmpFiles/Moving.json");

    CROW_ROUTE(app, "/search/<string>")
        .methods("GET"_method, "OPTIONS"_method)
        ([&automaton](const crow::request& req, crow::response& res, const std::string& w) {
        if (req.method == crow::HTTPMethod::Options) {
            res.code = 204;
            return res.end();
        }

        res.set_header("Content-Type", "application/json");

        std::string word = w;
        auto mv = automaton.SearchVertex(word);
        if (mv) {
            res.write(mv->ToJson(word).dump());
            return res.end();
        }

        //  נסיון להוריד מהמילה את האותיות התחיליות- מוספיות כדי למצוא אותה באוטומט
        bool found_with_motion = false;
        auto original = word;
        const std::string letters = "muefklb";
        while (!word.empty() && letters.find(word[0]) != std::string::npos) {
            word = word.substr(1);
            mv = automaton.SearchVertex(word);
            if (mv) {
                res.write(mv->ToJson(word).dump());
                return res.end();
            }
        }

        // אם לא מצאנו את המילה,נאיית אותה , נחזיר מערך תנועות של אותיות המילה לחיפוש
        json array_result = json::array();
        for (char c : original) {
            std::string ch(1, c);
            mv = automaton.SearchVertex(ch);
            if (mv) {
                array_result.push_back(mv->ToJson(ch));
            }
            else {
                array_result.push_back({
                    {"status", "not_found"},
                    {"word", ch}
                    });
            }
        }

        json result = {
            {"status", "partial_letters"},
            {"word", original},
            {"segments", array_result}
        };
        res.write(result.dump());
        return res.end();
            });

    app.port(18080).multithreaded().run();
}
