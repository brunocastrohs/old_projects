import './MouseWatcher.css';
import React, { useEffect, useRef } from 'react';
import MousePosition from 'ol/control/MousePosition';
import {createStringXY} from 'ol/coordinate';


function MouseWatcher(props) {

  const element = useRef();

  useEffect( () => {

    const control = new MousePosition({
      coordinateFormat: createStringXY(4),
      projection: 'EPSG:4326',
      placeholder:'0, 0',
      className: 'custom-mouse-position',
      target: element.current,
    });

    props.map.addControl(control);
    
  },[props])
 
  return (   
    <div className="MouseWatcher">
      <div ref={element} className="MouseWatcherContent"> </div>
    </div>  
  ) 

}

export default MouseWatcher;