import { ServerTypeHelper, LayerFolderHelper } from '../helpers';

const esriLayers = [
    {
      wmsName: '',
      title: 'World Topo Map',
      url: 'https://server.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/{z}/{y}/{x}',
      attributions: 'Tiles © <a href="https://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer">ArcGIS</a>',
      visibility: true,
      queryable: false,
      serverType: ServerTypeHelper.ESRI,
      folder: LayerFolderHelper.ESRI,
    },
    {
      wmsName: '',
      title: 'Satellite',
      url: 'https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',
      attributions: 'Tiles © <a href="https://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer">ArcGIS</a>',
      visibility: false,
      queryable: false,
      serverType: ServerTypeHelper.ESRI,
      folder: LayerFolderHelper.ESRI,
    },
    {
      wmsName: '',
      title: 'Streets',
      url: 'https://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer/tile/{z}/{y}/{x}',
      attributions: 'Tiles © <a href="https://services.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer">ArcGIS</a>',
      visibility: false,
      queryable: false,
      serverType: ServerTypeHelper.ESRI,
      folder: LayerFolderHelper.ESRI,
    },
  ];

  export default esriLayers;