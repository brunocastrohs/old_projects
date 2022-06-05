import React from 'react';
import './LegendGraphic.css';


const LegendGraphic = (props) => {

  const htmlElement = useRef();

  const handleClick = () =>{
    props.map.getView().setZoom(zoom);
    props.map.getView().setCenter(center);
    //props.map.un('singleclick',localStorage.getItem('singleclick'));
  }
 
  return (   
    <><input type='image' src={restoreView} title='Expandir zoom.' onClick={handleClick} ref={htmlElement} className="navbar-toggler RestoreView"></input></>
  ) ;
  
}


export default LegendGraphic;
