import React, { useEffect, useState } from 'react';
import { Canvas } from '@react-three/fiber';
import { OrbitControls, useGLTF, useAnimations } from '@react-three/drei';

function Model({ url, playAnimation }) {
  const { scene, animations } = useGLTF(url);
  const { actions } = useAnimations(animations, scene);

  useEffect(() => {
    if (playAnimation && actions && Object.keys(actions).length > 0) {
      Object.values(actions).forEach(action => action.reset().play());
      return () => Object.values(actions).forEach(action => action.stop());
    }
  }, [actions, animations, playAnimation]);

  return <primitive object={scene} scale={[5, 5, 5]} position={[1, -5.5, 0]} />;
}

export default function AnimationDisplay({ animationQueue, onRemoveFirstAnimation }) {
  const neutralModelUrl = "/natural.glb";
  const [currentUrl, setCurrentUrl] = useState(neutralModelUrl);
  const [playAnimation, setPlayAnimation] = useState(false);

  useEffect(() => {
    if (animationQueue.length > 0) {
      setCurrentUrl(animationQueue[0]);
      setPlayAnimation(true);
      const timer = setTimeout(() => {
        onRemoveFirstAnimation();
      }, 3000); // זמן תקורה בין אנימציות
      return () => clearTimeout(timer);
    } else {
      setCurrentUrl(neutralModelUrl);
      setPlayAnimation(false);
    }
  }, [animationQueue, onRemoveFirstAnimation, neutralModelUrl]);

  return (
    <div style={{ width: '1000px', margin: 'auto' }}>
      <div style={{ height: '500px' }}>
        <Canvas width="100%" height="100%">
          <ambientLight intensity={0.5} />
          <directionalLight position={[10, 10, 5]} />
          <spotLight
            position={[0, 2, 5]}
            angle={0.4}
            intensity={2}
            penumbra={1}
            castShadow
            target-position={[0, 0, 0]}
          />
          <React.Suspense fallback={null}>
            <Model key={currentUrl} url={currentUrl} playAnimation={playAnimation} />
            <OrbitControls />
          </React.Suspense>
        </Canvas>
      </div>
    </div>
  );
}