import React,{useState} from 'react';
import './Toolbar.css';
import PngExporter from '../../../PngExporter';
import FeatureInfo from '../../../FeatureInfo';
import DistanceMeasurer from '../../../DistanceMeasurer';
import AreaMeasurer from '../../../AreaMeasurer';



const Toolbar = (props) => {

  const [activeTool, setActiveTool] = useState(null);

  const handleActiveTool = (instance)=>{
    setActiveTool(instance);
  }  

  return (
    <div className="Toolbar d-flex justify-content-center">
              <FeatureInfo map={props.map} handleActiveTool={handleActiveTool} activeTool={activeTool}/>
              <PngExporter map={props.map} handleActiveTool={handleActiveTool} activeTool={activeTool}/> 
              <DistanceMeasurer map={props.map} handleActiveTool={handleActiveTool} activeTool={activeTool}/> 
              <AreaMeasurer map={props.map} handleActiveTool={handleActiveTool} activeTool={activeTool}/> 
    </div>);
}


export default Toolbar;
