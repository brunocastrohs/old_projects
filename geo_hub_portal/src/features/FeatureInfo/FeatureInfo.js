import './FeatureInfo.css';
import React, { useState, useEffect, useRef } from 'react';
import { toLonLat } from 'ol/proj';
import { createTitle, createRecord } from '../../utils/JSXHandler';
import TemplateModal from '../../components/TemplateModal/TemplateModal';
import featureInfo from '../../_assets/img/featureInfo.png';
import featureInfo_min from '../../_assets/img/featureInfo_min.png';

function FeatureInfo(props) {

  const htmlElement = useRef();

  const [modalValue, setModalValue] = useState('');

  const [active, setActive] = useState(false);

  const clientWidth = document.documentElement.clientWidth;

  const checkQueryableLayers = () => {

    let queryableLayers = [];

    for (let i = 0; i < props.map.getLayers().getArray().length; i++) {
      if (props.map.getLayers().getArray()[i].getVisible() && props.map.getLayers().getArray()[i].get('queryable')) {
        queryableLayers.push(props.map.getLayers().getArray()[i]);
      }
    }

    return queryableLayers;

  }

  const requestFeatureInfo = async (evt, queryableLayers) => {

    let featureInfo = [];

    for (let i = 0; i < queryableLayers.length; i++) {
      let view = props.map.getView();
      let viewResolution = view.getResolution();
      let source = queryableLayers[i].getSource();
      let url = source?.getFeatureInfoUrl(
        evt.coordinate, viewResolution, view.getProjection(),
        { 'INFO_FORMAT': 'application/json', 'FEATURE_COUNT': 50 });
      if (url) {
        await fetch(url)
          .then((response) => {
            return response.json().then((data) => {
              if (data && data.features && data.features.length > 0) {
                featureInfo.push(createTitle(queryableLayers[i].get('title')));
                for (let i = 0; i < data.features.length; i++)
                  featureInfo.push(createRecord(i, data.features[i].properties));
                return true;
              }
              else
                return false;
            }).catch((err) => {
              console.log(err);
            })
          });

      }
    }

    return featureInfo;

  }


  useEffect(() => {

    window.activeFillModal = false;

    props.map.on('singleclick',
      async (evt) => {
        if (window.activeFillModal) {

          const coordinate = toLonLat(evt.coordinate);

          const coordinateWarning = (<span key='featureCoordinates'><h5>Coordenadas do click</h5><span className='FeatureField'> Latitude</span>: {(coordinate[0].toFixed(5))}, <span className='FeatureField'>Longitude</span>: {(coordinate[1].toFixed(5))}</span>);

          let queryableLayers = checkQueryableLayers();

          let featureInfo = await requestFeatureInfo(evt, queryableLayers);

          if (featureInfo.length > 0) {
            setModalValue([...featureInfo, (<hr key='format' />), coordinateWarning]);
          }

        }
      }
    );

  }, [props.map])

  useEffect(() => {

    window.activeFillModal = active;

  }, [active])

  useEffect(() => {

    if (props.activeTool !== htmlElement)
      setActive(false);


  }, [props.activeTool])

  const handleClick = () => {

    setActive(!active);
    props.handleActiveTool(htmlElement);

  }

  return (
    <>
      <input type='image' src={clientWidth > 500 ? featureInfo : featureInfo_min} alt='Informação' title='Informação por click.' onClick={handleClick} className={"navbar-toggler FeatureInfo " + (active ? "FeatureInfoActive" : "FeatureInfoDeactive")} ref={htmlElement}></input>
      <TemplateModal>{modalValue}</TemplateModal>
    </>
  )

}

export default FeatureInfo;