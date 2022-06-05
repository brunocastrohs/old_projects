import { ServerTypeHelper, LayerFolderHelper } from '../helpers';

const osmLayers = [
    {
      wmsName: '',
      title: 'Mapnick',
      url: 'http://tile.openstreetmap.org/{z}/{x}/{y}.png',
      visibility: false,
      queryable: false,
      serverType: ServerTypeHelper.OSM,
      folder: LayerFolderHelper.OSM,
    },
  ];


export default osmLayers;