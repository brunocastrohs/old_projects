import React from 'react';
import './Section.css';

const Section = (props) => {

  const {children} = props;
  
  return (

      <div className="row">
        {children}
      </div>
    );

}


export default Section;
