import Map from 'ol/Map';
import View from 'ol/View';
import {layers, explorer} from './layers/';

const zoom = 7.5;
const center = [-4404499.022, -557518.59];
const projection = 'EPSG:900913';

const map = new Map({
   layers: layers,
    view: new View({
      projection: projection,
      center: center,
      zoom: zoom
    }),
    controls: [],
  });

  export {map, zoom, center, projection, explorer};
