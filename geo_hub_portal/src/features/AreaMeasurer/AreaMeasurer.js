import './AreaMeasurer.css';
import Overlay from 'ol/Overlay';
import { Circle as CircleStyle, Fill, Stroke, Style } from 'ol/style';
import { getArea } from 'ol/sphere';
import { unByKey } from 'ol/Observable';
import Draw from 'ol/interaction/Draw';
import React, { useState, useEffect, useRef } from 'react';
import { source } from '../../_config/layers/vector';
import areaMeasurer from '../../_assets/img/areaMeasurer.png';
import areaMeasurer_min from '../../_assets/img/areaMeasurer_min.png';

let helpMsgArea = '';

let drawArea;

let sketchArea;

let helpTooltipElementArea;

let helpTooltipArea;

let measureTooltipElementArea = null;

let measureTooltipArea;

const initMsg = 'Clique para desenhar.';

const continueMsg = 'Clique para continuar desenhando.';

function DistanceMeasurer(props) {

  const htmlElement = useRef();

  const clientWidth = document.documentElement.clientWidth;

  const [active, setActive] = useState(false);

  const formatArea = function (polygon) {
    const area = getArea(polygon);
    let output;
    if (area > 10000) {
      output = Math.round((area / 1000000) * 100) / 100 + ' ' + 'km<sup>2</sup>';
    } else {
      output = Math.round(area * 100) / 100 + ' ' + 'm<sup>2</sup>';
    }
    return output;
  };
  

  const pointerMoveHandler = function (evt) {
    if (evt.dragging) {
      return;
    }

    if (sketchArea) {

      helpMsgArea = continueMsg;

    }

    helpTooltipElementArea.innerHTML = helpMsgArea;
    helpTooltipArea.setPosition(evt.coordinate);

    helpTooltipElementArea.classList.remove('hidden');
  };

  function addInteraction() {

    const type = 'Polygon';
    drawArea = new Draw({
      source: source,
      type: type,
      style: new Style({
        fill: new Fill({
          color: 'rgba(255, 255, 255, 0.2)',
        }),
        stroke: new Stroke({
          color: 'rgba(0, 0, 0, 0.5)',
          lineDash: [10, 10],
          width: 2,
        }),
        image: new CircleStyle({
          radius: 5,
          stroke: new Stroke({
            color: 'rgba(0, 0, 0, 0.7)',
          }),
          fill: new Fill({
            color: 'rgba(255, 255, 255, 0.2)',
          }),
        }),
      }),
    });

    props.map.addInteraction(drawArea);

    createMeasureTooltip();
    createHelpTooltip();

    let listener;
    drawArea.on('drawstart', function (evt) {
      // set sketchArea
      sketchArea = evt.feature;

      let tooltipCoord = evt.coordinate;

      listener = sketchArea.getGeometry().on('change', function (evt) {
        const geom = evt.target;
        let output;
        output = formatArea(geom);
        tooltipCoord = geom.getInteriorPoint().getCoordinates();
        measureTooltipElementArea.innerHTML = output;
        measureTooltipArea.setPosition(tooltipCoord);
      });
    });

    drawArea.on('drawend', function () {
      measureTooltipElementArea.className = 'ol-tooltip ol-tooltip-static';
      measureTooltipArea.setOffset([0, -7]);
      // unset sketchArea
      sketchArea = null;
      // unset tooltip so that a new one can be created
      measureTooltipElementArea = null;
      createMeasureTooltip();
      unByKey(listener);
    });
  }

  function createHelpTooltip() {
    if (helpTooltipElementArea) {
      helpTooltipElementArea.parentNode.removeChild(helpTooltipElementArea);
    }
    helpTooltipElementArea = document.createElement('div');
    helpTooltipElementArea.className = 'ol-tooltip hidden';
    helpTooltipArea = new Overlay({
      element: helpTooltipElementArea,
      offset: [15, 0],
      positioning: 'center-left',
    });
    props.map.addOverlay(helpTooltipArea);
  }

  function createMeasureTooltip() {
   
    measureTooltipElementArea = document.createElement('div');
    measureTooltipElementArea.className = 'ol-tooltip ol-tooltip-measure';
    measureTooltipArea = new Overlay({
      element: measureTooltipElementArea,
      offset: [0, -15],
      positioning: 'bottom-center',
      stopEvent: false,
      insertFirst: false,
    });
    props.map.addOverlay(measureTooltipArea);
  }

  const handleActivity = (activate) => {
    if (activate) {
      helpMsgArea = initMsg;
      props.map.on('pointermove', pointerMoveHandler);

      props.map.getViewport().addEventListener('mouseout', function () {
        helpTooltipElementArea.classList.add('hidden');
      });

      addInteraction();
      setActive(true);
    }
    else {
      props.map.removeInteraction(drawArea);
      helpMsgArea = '';
      setActive(false);
      source.clear();
      const tooltips = document.querySelectorAll('.ol-tooltip-static');
      if(tooltips && tooltips.length!=0){
        for (let i = 0; i < tooltips.length; i++) {
          tooltips[i].remove();
        }
      }
    }

  }

  useEffect(() => {


    handleActivity(active)


  }, [active])

  useEffect(() => {

    if (props.activeTool !== htmlElement)
      handleActivity(false)


  }, [props.activeTool])

  const handleClick = () => {

    setActive(!active);
    props.handleActiveTool(htmlElement);

  }

  return (
    <>
      <input type='image' alt='Mensurar área em km<sup>2</sup>.' src={clientWidth > 500 ? areaMeasurer : areaMeasurer_min} title='Mensurar distância.' onClick={handleClick} ref={htmlElement} className={"navbar-toggler AreaMeasurer " + (active ? "AreaMeasurerActive" : "AreaMeasurerDeactive")}></input>
    </>
  )
}

export default DistanceMeasurer;