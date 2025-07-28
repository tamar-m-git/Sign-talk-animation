import { useState } from 'react';
import CameraRecorder from './components/CameraRecorder';
import AnimationDisplay from './components/AnimationDisplay';
import './App.css';

function App() {
const [started, setStarted] = useState(false);
const [animationQueue, setAnimationQueue] = useState([]);
const clearQueue = () => setAnimationQueue([]);
const removeFirstAnimation = () => setAnimationQueue(q => q.slice(1));
const handleStart = async () => {
    await fetch("http://localhost:5005/clear_output", { method: "POST" });
    setStarted(true);
  };
  const overlayStyle = started
    ? { display: 'none' }
    : {
        position: 'fixed',
        top: 0,
        left: 0,
        width: '100%',
        height: '100%',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        justifyContent: 'center',
        backgroundColor: 'rgba(255,255,255,0.9)',
      };

  return (
    <div className="app">
      

      <div style={overlayStyle}>
        <img
          src="/LogoSighns.png"
          alt="לוגו"
          style={{ width: '500px', marginBottom: '30px' }}
        />
        <button className="start-button" onClick={() => setStarted(true)}>
          התחל שיחה
        </button>
      </div>


      {started && (
  <div style={{
    width: '100vw',
    height: '100vh',
    position: 'relative',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    background: '#f5f5f5'
  }}>
    <CameraRecorder
      onAnimationReady={(url) => {
        setAnimationQueue((prev) => [...prev, url]);
      }}
    />
<div
  style={{
    position: 'absolute',
    left: 400,
    bottom: 150,
    width: 400,
    height: 300,
    background: '#fff',
    borderRadius: 12,
    boxShadow: '0 0 10px #0002',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    zIndex: 10,
    overflow: 'hidden',
    transform: 'scale(0.7)',       
    transformOrigin: 'bottom left',    
  }}
>
  <AnimationDisplay
    animationQueue={animationQueue}
    onRemoveFirstAnimation={removeFirstAnimation}
    onClearQueue={clearQueue}
  />
</div>
  </div>
)}
    </div>
  );
}

export default App;