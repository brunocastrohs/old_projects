import React, { useRef, useState } from 'react';
import ImageWMS from 'ol/source/ImageWMS';

import { ServerTypeHelper } from '../../../../_config/helpers';
import './LayerFolder.css';
import openedFolder from '../../../../_assets/img/openedFolder.png';
import closedFolder from '../../../../_assets/img/closedFolder.png';


const LayerFolder = (props) => {

  const content = useRef();

  const titleUnicode = useRef();


  const [toggle, setToggle] = useState(false);

  const handleLayerClick = (event) => {

    const index = event.target.value;

    const layer = props.map.getLayers().getArray()[index];

    layer.setVisible(!layer.getVisible());

    handleGetLegendGraphic(layer, index);

    props.handleLayerZIndexer(layer);

  }

  const handleGetLegendGraphic = (layer, index) => {

    if (layer.getVisible() && (layer.get('serverType') === ServerTypeHelper.GEOSERVER || layer.get('serverType') === ServerTypeHelper.MAPSERVER)) {

      const wmsSource = new ImageWMS({
        url: layer.get('url'),
        params: { 'LAYERS': layer.get('wmsName') },
        ratio: 1,
        serverType: layer.get('serverType'),
      });

      const graphicUrl =
        wmsSource.getLegendUrl(props.map.getView().getResolution(),
          { "legend_options": "bgColor:#212835;fontColor:#FFFFFF" },
        );
      const img = document.getElementById('layerLegend' + index);
      const legendTitle = document.getElementById('layerLegendTitle' + index);
      img.src = graphicUrl;
      img.style.visibility = 'visible';
      img.style.display = 'inherit';
      legendTitle.style.visibility = 'visible';
      legendTitle.style.display = 'inherit';
    } else if (!layer.getVisible()) {
      const legendTitle = document.getElementById('layerLegendTitle' + index);
      const img = document.getElementById('layerLegend' + index);
      img.style.visibility = 'hidden';
      img.style.display = 'none';
      legendTitle.style.visibility = 'hidden';
      legendTitle.style.display = 'none';
    }


  }

  const handleFolderClick = () => {

    if (toggle) {
      content.current.style.visibility = 'hidden';

      content.current.style.display = 'none';

      content.current.style.opacity = '0';

      setToggle(false);
    } else {
      content.current.style.visibility = 'visible';

      content.current.style.display = 'inherit';

      content.current.style.opacity = '1';

      setToggle(true);
    }
  }

  return (
    <div className='folder'>
      <h3 className='title' onClick={handleFolderClick}><div ref={titleUnicode}><img alt="Pasta" src={toggle ? openedFolder : closedFolder} /></div> {props.title}</h3>
      <div ref={content} className='content'>
        {props.map.getLayers().getArray().map((layer, index) =>
          layer.get('folder') === props.title ?

            <div className='item' key={index}>

              <input className='layerCheck form-check-input' type='checkbox' defaultChecked={layer.getVisible() && layer.get('title') !== 'Grade'} id={'layer' + index} value={index} onClick={handleLayerClick} />

              <label className='form-check-label' htmlFor={'layer' + index}>
                <h5 className='layerTitle'>🗺️ {layer.get('title')}</h5>
                <span id={'layerLegendTitle' + index} className='LayerLegendTitle'>Legenda: </span>
                <img alt="Pasta" id={'layerLegend' + index} className='LayerLegend' />
              </label>

            </div>
            : null
        )}
      </div>
    </div>
  );
}


export default LayerFolder;
