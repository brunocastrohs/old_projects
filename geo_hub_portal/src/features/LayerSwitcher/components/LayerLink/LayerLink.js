import React from 'react';
import './LayerLink.css';


const LayerLink = (props) => {

  return (
      <>
      <span className="LayerLink" href={props.href} onClick={props.callback} >{props.content}</span>
      <br/><br/>
      </>
    );
}


export default LayerLink;
