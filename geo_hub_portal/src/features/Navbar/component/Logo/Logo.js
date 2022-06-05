import React from 'react';
import './Logo.css';

const Logo = () => {
  
  return (
    <a href='localhost:3000' className='navbar-brand Logo'><h2>{process.env.REACT_APP_TITLE} <span>{process.env.REACT_APP_TITLE_FULL}</span></h2></a>
    );

}


export default Logo;
