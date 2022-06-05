import React from 'react';
import './Navbar.css';
import Logo from './component/Logo';
import Toggler from './component/Toggler';
import Toolbar from './component/Toolbar';
import TemplateModal from '../../components/TemplateModal';

const Navbar = (props) => {
  
  return (
    <nav className="navbar navbar-dark Navbar">
      <div className="container-fluid">
            <Logo/>
            <Toolbar map={props.map}/>
            <Toggler switcherRef={props.switcherRef}/>
            <TemplateModal />
      </div>
    </nav>
    );
}


export default Navbar;
