import React from 'react';
import './Footer.css';

const Footer = (props) => {
  
  return (
      <div className="container justify-center">
        <h4>{process.env.REACT_APP_FOOTER}</h4>
      </div>
    );


}


export default Footer;
