// src/utils/transliterate.js

// מפה של אותיות עבריות לאותיות לטיניות
const hebrewMap = {
  'א': 'a', 'ב': 'b', 'ג': 'c', 'ד': 'd', 'ה': 'e',
  'ו': 'f', 'ז': 'g', 'ח': 'h', 'ט': 'i', 'י': 'j',
  'כ': 'k', 'ך': 'k', 'ל': 'l', 'מ': 'm', 'ם': 'm',
  'נ': 'n', 'ן': 'n', 'ס': 'o', 'ע': 'p', 'פ': 'q',
  'ף': 'q', 'צ': 'r', 'ץ': 'r', 'ק': 's', 'ר': 't',
  'ש': 'u', 'ת': 'v'
};

// src/utils/constants.js
export const noMotionWords = new Set([
  'ףא', 'רמולכ', 'ךכיפל', 'ירה', 'קרו', 'ךא', 'זא', 'את'
]);


/**
 * מתרגמת מילה בעברית לרשומה לטינית לפי המפה.
 * @param {string} word — מילה בעברית
 * @returns {string} — התרגום האותי
 * @throws {Error} — אם נמצא תו שאינו אות עברית
 */
export function transliterateHebrew(word) {
  let result = '';
  for (const char of word) {
    const mapped = hebrewMap[char];
    if (!mapped) {
      throw new Error(`Invalid character '${char}': not a Hebrew letter`);
    }
    result += mapped;
  }
  return result;
}
