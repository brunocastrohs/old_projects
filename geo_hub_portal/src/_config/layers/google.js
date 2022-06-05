import { ServerTypeHelper, LayerFolderHelper } from '../helpers';

const googleLayers = [
  {
    wmsName: '',
    title: 'Satellite only',
    url: 'http://mt0.google.com/vt/lyrs=s&hl=en&x={x}&y={y}&z={z}',
    visibility: false,
    queryable: false,
    serverType: ServerTypeHelper.GOOGLE,
    folder: LayerFolderHelper.GOOGLE,
  },
      
  {
        wmsName: '',
        title: 'Streets',
        url: 'http://mt0.google.com/vt/lyrs=p&hl=en&x={x}&y={y}&z={z}',
        visibility: false,
        queryable: false,
        serverType: ServerTypeHelper.GOOGLE,
        folder: LayerFolderHelper.GOOGLE,
    },  
    {
      wmsName: '',
      title: 'Hybrid',
      url: 'http://mt0.google.com/vt/lyrs=y&hl=en&x={x}&y={y}&z={z}',
      visibility: false,
      queryable: false,
      serverType: ServerTypeHelper.GOOGLE,
      folder: LayerFolderHelper.GOOGLE,
    }
];

export default googleLayers;