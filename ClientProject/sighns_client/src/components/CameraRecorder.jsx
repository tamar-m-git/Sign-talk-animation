import React, { useEffect, useState, useRef } from 'react';
import Webcam from 'react-webcam';
import { transliterateHebrew } from '../TmpFunction';

export default function CameraRecorder({ onAnimationReady, onStop }) {
  const [sentence, setSentence] = useState('');
  const [responses, setResponses] = useState([]);
  const [cameraOn, setCameraOn] = useState(true);
const [micOn, setMicOn] = useState(true);

  const queueRef = useRef([]);
  const processingRef = useRef(false);
   const urlQueueRef = useRef([]);        
  const urlProcessingRef = useRef(false);
  const recognitionRef = useRef(null);
  const webcamRef = useRef(null);


  async function sendWord(word) {
    try {
      const res = await fetch(`http://localhost:18080/search/${word}`);
      return await res.json();
    } catch {
      return { status: 'error', word };
    }
  }

  function toggleCamera() {
  setCameraOn(prev => !prev);
}

function toggleMic() {
  setMicOn(prev => !prev);
  if (recognitionRef.current) {
    if (micOn) {
      recognitionRef.current.stop();
    } else {
      recognitionRef.current.start();
    }
  }
}

function handleCallEnd() {
  // עצירת ההקלטה וניהול מעבר לדף כניסה
  if (recognitionRef.current) recognitionRef.current.stop();
  if (onStop) onStop();
   window.location.href = '/';
  // לדוגמה, ניווט חזרה לדף כניסה (אם יש לך ראוטר)
  // window.location.href = '/login'; // או נתיב כניסה אחר
}

async function requestAnimation(motionData, actionName) {
  try {
    const res = await fetch("http://localhost:5005/create_animation", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ motion_data: motionData, action_name: actionName }),
    });
    const data = await res.json();
    console.log("🎬 תגובת שרת ליצירת אנימציה:", data); // כאן תראי את כל התגובה מהשרת
if (data.status === "ok") {
  console.log("🌐 כתובת האנימציה שהתקבלה:", data.url); // כאן תראי את ה-URL עצמו
  console.log(`✅ SuccesAnmation for word: ${actionName}`);
  return data.url;
}
    throw new Error(data.error || 'Unknown error');
  } catch (err) {
    console.error("Error creating animation:", err);
    return null;
  }
}
function processUrlQueue() {
  if (urlProcessingRef.current || urlQueueRef.current.length === 0) return;
  urlProcessingRef.current = true;

  const url = urlQueueRef.current.shift();
  if (onAnimationReady) {
    onAnimationReady(url); // תפעילי את האנימציה/הצגה
  }

  urlProcessingRef.current = false;
  // אם יש עוד כתובות בתור, המשיכי לעבד
  if (urlQueueRef.current.length > 0) {
    processUrlQueue();
  }
}

async function processQueue() {
  if (processingRef.current || queueRef.current.length === 0) return;
  processingRef.current = true;

  while (queueRef.current.length) {
    const word = queueRef.current.shift();
    const response = await sendWord(word);

    if (response.status === 'partial_letters') {
      for (const segment of response.segments) {
        setResponses(prev => [...prev, { word: segment.word, status: segment.status }]);
        if (segment.status === 'found_with_motion') {
          const animationPath = await requestAnimation(segment.motion_data, segment.word);
          if (animationPath) {
            urlQueueRef.current.push(animationPath);
            console.log("📋 תור ה-URL הנוכחי:", urlQueueRef.current);
             processUrlQueue();  // מחק/י שורה זו
          }
        }
      }
    } else {
      setResponses(prev => [...prev, { word: response.word, status: response.status }]);
      if (response.status === 'found_with_motion') {
        const animationPath = await requestAnimation(response.motion_data, response.word);
        if (animationPath) {
          urlQueueRef.current.push(animationPath);
          console.log("📋 תור ה-URL הנוכחי:", urlQueueRef.current);
           processUrlQueue();  // מחק/י שורה זו
        }
      }
    }

    await new Promise(r => setTimeout(r, 100));
  }

  processingRef.current = false;
}



  useEffect(() => {
    const SR = window.SpeechRecognition || window.webkitSpeechRecognition;
    if (!SR) {
      alert('הדפדפן שלך לא תומך בזיהוי קולי');
      return;
    }

    const recognition = new SR();
    recognition.lang = 'he-IL';
    recognition.continuous = true;
    recognition.interimResults = false;

    recognition.onresult = e => {
      const text = e.results[e.resultIndex][0].transcript.trim();
      setSentence(text);
      const words = text.split(/\s+/).map(w => {
        try { return transliterateHebrew(w); }
        catch { return null; }
      }).filter(Boolean);
      queueRef.current.push(...words);
      processQueue();
    };

    recognition.start();
    recognitionRef.current = recognition;

    return () => recognition.stop();
  }, []);

  function handleStop() {
    if (recognitionRef.current) recognitionRef.current.stop();
    if (onStop) onStop();
  }

  return (
<div style={{ textAlign: 'center', position: 'relative' ,width: '55%',height: '80%'}}>
  <div
    style={{
      width: '80%%',
      height: '90%',
      position: 'relative',
      backgroundColor: '#000',
       alignItems: 'center',      // מרכז אנכית
    justifyContent: 'center',
      borderRadius: '16px', // פינות עגולות
      boxShadow: '0 4px 12px rgba(0, 0, 0, 0.5)', // צל שחור
      overflow: 'hidden', // חשוב בשביל הפינות העגולות
    }}
  >
    <Webcam
      audio={false}
      ref={webcamRef}
      screenshotFormat="image/jpeg"
      style={{
        width: '100%',
        height: '100%',
        objectFit: 'cover',
        display: cameraOn ? 'block' : 'none',
      }}
    />
    {!cameraOn && (
      <div
        style={{
          position: 'absolute',
          top: 0,
          left: 0,
          width: '100%',
          height: '100%',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          color: '#fff',
          backgroundColor: '#000',
        }}
      >
        <img src="/no-video.png" alt="Camera Off" style={{ width: '100px', opacity: 0.5 }} />
      </div>
    )}
  </div>




<div
  style={{
    marginTop: '10px',
    display: 'flex',
    justifyContent: 'center',
    gap: '20px',
    backgroundColor: 'transparent',
    padding: '0',
  }}
>
  <button
    onClick={toggleMic}
    style={{
      border: 'none',
      borderRadius: '50%',
      width: '50px',
      height: '50px',
      cursor: 'pointer',
      backgroundColor: 'white',
      boxShadow: '0 2px 6px rgba(0,0,0,0.15)',
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
    }}
    title={micOn ? 'כבה מיקרופון' : 'הדלק מיקרופון'}
  >
<img
  src={micOn ? "/microphone.png" : "/mute (2).png"}
  alt={micOn ? "Microphone On" : "Microphone Off"}
  style={{ width: '30px', height: '30px' }}
/>

  </button>

  <button
    onClick={handleCallEnd}
    style={{
      border: 'none',
      borderRadius: '50%',
      width: '50px',
      height: '50px',
      cursor: 'pointer',
      backgroundColor: 'white',
      boxShadow: '0 2px 6px rgba(0,0,0,0.15)',
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
    }}
    title="ניתוק שיחה"
  >
    <img src="/phone-call.png" alt="End Call" style={{ width: '30px', height: '30px' }} />
  </button>

  <button
    onClick={toggleCamera}
    style={{
      border: 'none',
      borderRadius: '50%',
      width: '50px',
      height: '50px',
      cursor: 'pointer',
      backgroundColor: 'white',
      boxShadow: '0 2px 6px rgba(0,0,0,0.15)',
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
    }}
    title={cameraOn ? 'כבה מצלמה' : 'הדלק מצלמה'}
  >
<img
  src={cameraOn ? "/video-camera.png" : "/no-video.png"}
  alt={cameraOn ? "Camera On" : "Camera Off"}
  style={{ width: '30px', height: '30px' }}
/>

  </button>
</div>



      <h4>המשפט האחרון:</h4>
      <p>{sentence || 'טרם נאמר משהו...'}</p>
      <h4>תשובות מילים:</h4>
      <ul style={{ textAlign: 'left', display: 'inline-block' }}>
        {responses.map((r, i) => (
          <li key={i}>
            <b>{r.word}</b>: {statusLabel(r.status)}
          </li>
        ))}
      </ul>
    </div>
  );
}



function statusLabel(status) {
  switch (status) {
    case 'found_with_motion': return ' נמצאה עם תנועה';
    case 'found_no_motion': return ' נמצאה ללא תנועה';
    case 'not_found': return ' לא נמצאה';
    case 'partial_letters': return ' פירוק לאותיות';
    case 'unknown': return ' מצב לא ידוע';
    default: return ' שגיאה';
  }
}
