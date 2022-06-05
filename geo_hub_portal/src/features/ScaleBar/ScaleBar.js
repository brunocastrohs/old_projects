import './ScaleBar.css';
import React, { useEffect, useRef } from 'react';
import ScaleLine from 'ol/control/ScaleLine'

function ScaleBar(props) {

  const scaleBarElement = useRef();

  useEffect( () => {

    const control = new ScaleLine({className: 'ol-scale-line', target: scaleBarElement.current});
    
    props.map.addControl(control);
    
  },[props])
 
  return (   
    <div className="ScaleBar">
      <div ref={scaleBarElement} className="ScaleBarContent"> </div>
    </div>
      
  ) 

}

export default ScaleBar