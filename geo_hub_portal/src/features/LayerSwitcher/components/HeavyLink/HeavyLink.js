import React from 'react';
import './HeavyLink.css';


const HeavyLink = (props) => {

  return (
      <>
      <a className="btn btn-outline-light btn-lg HeavyLink" href={props.href} target='_blank' rel="noreferrer">{props.content}</a>
      <br/><br/>
      </>
    );
}


export default HeavyLink;
