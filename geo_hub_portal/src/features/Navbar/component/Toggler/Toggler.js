import React from 'react';
import './Toggler.css';

const Toggler = (props) => {

  const handleClick = () => {

    const element = props.switcherRef.current;

    element.style.visibility = 'visible';       

    element.style.opacity = '1';
    
    element.style.display = 'block';

  }
  
  return (
    <button className="navbar-toggler Toggler" type="button" onClick={handleClick} >
      ☰
    </button>
    );

}


export default Toggler;
