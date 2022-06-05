import './ZoomTarget.css';
import React, { useEffect, useRef } from 'react';
import Zoom from 'ol/control/Zoom';
import {zoom, center} from '../../_config/map';
import restoreView_min from '../../_assets/img/restoreView_min.png';


function ZoomTarget(props) {

  const element = useRef();

  const restoreElement = useRef();

  useEffect( () => {

    const control = new Zoom({className: 'ol-zoom', target: element.current});
    
    props.map.addControl(control);
    
  },[props])
 
  const handleClick = () =>{
    props.map.getView().setZoom(zoom);
    props.map.getView().setCenter(center);
    //props.map.un('singleclick',localStorage.getItem('singleclick'));
  }

  return (   
    <div className="ZoomTarget">
      <input type='image' alt='Expandir zoom' src={restoreView_min} title='Expandir zoom.' onClick={handleClick} ref={restoreElement} className="RestoreView"></input>
      <div ref={element} className="ZoomTargetContent"> </div>
    </div>
  ) 

}

export default ZoomTarget;