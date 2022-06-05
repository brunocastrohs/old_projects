import React, {useState} from 'react';
import LayerFolder from './components/LayerFolder';
import Folder from './components/Folder';
import HeavyLink from './components/HeavyLink';
import LayerLink from './components/LayerLink';
import { ServerTypeHelper } from '../../_config/helpers';
import './LayerSwitcher.css';
import logo from '../../_assets/img/logo.png';
import close from '../../_assets/img/close.png';

const LayerSwitcher = (props) => {

  const title = 'Explorador de dados';

  const heavyLinkListTitle = 'Links de acesso';

  const [zIndexer, setZIndexer] = useState(props.map.getLayers().getArray().length);

  const handleLayerZIndexer = (layer) => {

    const vector = props.map.getLayers().getArray()[props.map.getLayers().getArray().length-2];
    const graticule = props.map.getLayers().getArray()[props.map.getLayers().getArray().length-1];

    if(layer.getVisible()){
      setZIndexer(zIndexer+1)
      layer.setZIndex(zIndexer);
      vector.setZIndex(zIndexer+1)
      graticule.setZIndex(zIndexer+2);
    }

  }

  const collapseLayerSwitcher = () => {

    const element = props.switcherRef.current;

    element.style.visibility = 'hidden';

    element.style.opacity = '0';

    element.style.display = 'none';

  }

  const buildFolderTree = (tree, index = 1) => {

    return tree.map((folder, index) => {
      if (folder.subFolder == null) {
        return (<LayerFolder handleLayerZIndexer={handleLayerZIndexer} title={folder.title} map={props.map} key={'folder' + index} />);
      }
       else {
        return (<Folder title={folder.title} map={props.map} key={'folder' + index}>{buildFolderTree(folder.subFolder, index + 1)}</Folder>);
      }
    }
    )

  }

  const deactivateVisibleLayers = () => {

    for(let i = 0; i < props.map.getLayers().getArray().length; i++){
      if(props.map.getLayers().getArray()[i].getVisible() && (props.map.getLayers().getArray()[i].get('serverType')&&props.map.getLayers().getArray()[i].get('serverType')===ServerTypeHelper.GEOSERVER)){
        props.map.getLayers().getArray()[i].setVisible(false);
        document.getElementById('layer'+i).checked = false;
      }
    }

    return false;

  }

  return (
    <div ref={props.switcherRef} onMouseLeave={collapseLayerSwitcher} className={"LayerSwitcher p-4"} id="navbarToggleExternalContent">
      
      <div className="p-4 logo">

        <img className='img-thumbnail' src={logo} alt='Logo' />

      </div>

      <div className="p-4 explorer">

        <hr />
        <div className='row'>
          <span className='SwitcherTitle'>
            {title} 
            <span onClick={collapseLayerSwitcher} className='SwitcherToggler'>
              <img title='Fechar explorador de camadas.' src={close} alt='Fechar!' />
            </span>
          </span> 
          
        </div>

        <br />

        {buildFolderTree(props.explorer)}

        <LayerLink content='Desativar camadas visíveis' callback={deactivateVisibleLayers}/>

        <hr />

        <div className='row'>
          <span className='SwitcherTitle'>
            {heavyLinkListTitle} 
          </span> 
        </div>

        <br />

        <HeavyLink content='Consultar site do Atlas' href='https://www.sema.ce.gov.br/89965-2/planejamento-costeiro-e-marinho-do-ceara/atlas-digital-costeiro-e-marinho-do-ceara/'  />

        <HeavyLink content='Consultar site do Programa CCMA' href='https://www.sema.ce.gov.br/89965-2/'  />

        <HeavyLink content='Consultar informações do ZEEC' href='https://www.sema.ce.gov.br/gerenciamento-costeiro/zoneamento-ecologico-economico-da-zona-costeira-zeec/documentos-previos-para-consulta-publica-do-zeec/'  />

        <HeavyLink content='Vídeo de apresentação da plataforma' href='https://www.youtube.com/watch?v=CsfDQKaJJXc'  />

        <HeavyLink content='Consultar manual do usuário' href='/PEDE_PORTAL/manual.pdf'  />

        <HeavyLink content='Download de dados pelo servidor geográfico' href='http://geohub.sema.ce.gov.br/geoserver/web/wicket/bookmarkable/org.geoserver.web.demo.MapPreviewPage'  />


      </div>
    </div>
  );

}


export default LayerSwitcher;
