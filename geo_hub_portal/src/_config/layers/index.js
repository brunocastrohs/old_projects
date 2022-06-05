import TileLayer from 'ol/layer/Tile';
import XYZ from 'ol/source/XYZ';
import TileWMS from 'ol/source/TileWMS';
import graticule from './graticule';
import {vector} from './vector';
import {baseLayers, baseTree} from './base';
import {layers as zcmLayers, tree as zcmTree} from './zcm';
import { ServerTypeHelper } from '../helpers';

  const layers = [];

  const explorer = [...zcmTree,...baseTree];

  const layersData = [...baseLayers, ...zcmLayers];

  const createLayerByType = (layer, zIndex) =>{
    if(layer.serverType === ServerTypeHelper.GOOGLE || layer.serverType === ServerTypeHelper.OSM)
      return new TileLayer({title:layer.title, serverType: layer.serverType, wmsName:layer.wmsName, url:layer.url, queryable:layer.queryable, visible:layer.visibility, zIndex: zIndex, folder:layer.folder, source: new XYZ({url: layer.url, crossOrigin: "Anonymous"})}); 
    else if(layer.serverType === ServerTypeHelper.ESRI)
      return new TileLayer({title:layer.title, serverType: layer.serverType, wmsName:layer.wmsName, url:layer.url, queryable:layer.queryable, visible:layer.visibility, zIndex: zIndex, folder:layer.folder, source: new XYZ({url: layer.url, attributions:layer.attributions, crossOrigin: "Anonymous"})}); 
    else if(layer.serverType === ServerTypeHelper.GEOSERVER)
      return new TileLayer({title:layer.title, serverType: layer.serverType, wmsName:layer.wmsName, url:layer.url, queryable:layer.queryable, visible:layer.visibility, zIndex: zIndex, folder:layer.folder, source: new TileWMS({title:layer.title, url:layer.url, crossOrigin: "Anonymous", params:{LAYERS:layer.wmsName, TILED:true}}) });
    else
      return null;
  }

  let zIndex = 0;

  for(let i = 0; i < layersData.length;i++){
    layers.push(createLayerByType(layersData[i], zIndex++));
  }

  vector.setZIndex(zIndex++)
  graticule.setZIndex(zIndex);

  layers.push(vector);

  layers.push(graticule);

  export {layers, explorer};