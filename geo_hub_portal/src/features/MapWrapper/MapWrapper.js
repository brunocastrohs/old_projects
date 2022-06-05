import './MapWrapper.css';
import React, { useEffect, useRef } from 'react';

function MapWrapper(props) {

  const mapElement = useRef()
  const mapRef = useRef()
  

  useEffect( () => {
    mapRef.current = props?.map;
    props?.map.setTarget(mapElement.current);
  },[props])

  
  return (      
    <div ref={mapElement} className="MapWrapper">

      {props.children}

    </div>
  ) 

}

export default MapWrapper;