import './Welcome.css';
import React,{useRef} from 'react';
import MapWrapper from '../MapWrapper/';
import ScaleBar from '../ScaleBar/';
import MouseWatcher from '../MouseWatcher';
import Header from '../../containers/Header';
import Navbar from '../Navbar/';
import LayerSwitcher from '../LayerSwitcher/';
import ZoomTarget from '../ZoomTarget/';
import {map, explorer} from '../../_config/map';

const Welcome = () => {

  const collapseSwitcher = true;
  const switcherRef = useRef();

  return (
    <div className="Welcome">
      <Header>
        <Navbar map={map} switcherRef={switcherRef}/>
      </Header>
      <MapWrapper map={map}>
          <ScaleBar map={map}/> 
          <MouseWatcher map={map}/>
          <ZoomTarget map={map}/> 
          <LayerSwitcher map={map} switcherRef={switcherRef} explorer={explorer} collapsed={collapseSwitcher}/>
      </MapWrapper>        
    </div>
  )
}

export default Welcome;