import React, {useRef, useState} from 'react';
import './Folder.css';
import openedFolder from '../../../../_assets/img/openedFolder.png';
import closedFolder from '../../../../_assets/img/closedFolder.png';


const Folder = (props) => {

  const content = useRef();

  const titleUnicode = useRef();

  const [toggle, setToggle] = useState(false);

  const handleFolderClick = () => {

    if(toggle){
      content.current.style.visibility = 'hidden';    

      content.current.style.display = 'none';

      content.current.style.opacity = '0'; 

      setToggle(false);
    }else{
      content.current.style.visibility = 'visible';
      
      content.current.style.display = 'inherit';

      content.current.style.opacity = '1'; 

      setToggle(true);
    }
  }

  return (
      <div className='folder'> 
        <h3 className='title' onClick={handleFolderClick}><div ref={titleUnicode}><img alt="Pasta" src={toggle?openedFolder:closedFolder}/></div> {props.title}</h3>
        <div ref={content} className='content'>
          {props.children}
        </div>
      </div>
    );
}


export default Folder;
