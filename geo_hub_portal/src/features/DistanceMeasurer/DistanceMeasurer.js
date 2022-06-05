import './DistanceMeasurer.css';
import Overlay from 'ol/Overlay';
import { Circle as CircleStyle, Fill, Stroke, Style } from 'ol/style';
import { LineString } from 'ol/geom';
import { getLength } from 'ol/sphere';
import { unByKey } from 'ol/Observable';
import Draw from 'ol/interaction/Draw';
import React, { useState, useEffect, useRef } from 'react';
import { source } from '../../_config/layers/vector';
import distanceMeasurer from '../../_assets/img/distanceMeasurer.png';
import distanceMeasurer_min from '../../_assets/img/distanceMeasurer_min.png';

let helpMsg = '';

let draw;

let sketch;

let helpTooltipElement;

let helpTooltip;

let measureTooltipElement;

let measureTooltip;

const initMsg = 'Clique para desenhar.';

const continueLineMsg = 'Clique para continuar desenhando.';

function DistanceMeasurer(props) {

  const htmlElement = useRef();

  const clientWidth = document.documentElement.clientWidth;

  const [active, setActive] = useState(false);

  const formatLength = function (line) {
    const length = getLength(line);
    let output;
    if (length > 100) {
      output = `${(Math.round((length / 1000) * 100) / 100)} km`;
    } else {
      output = `${(Math.round(length * 100) / 100)} m`;
    }
    return output;
  };

  const pointerMoveHandler = function (evt) {
    if (evt.dragging) {
      return;
    }

    if (sketch) {

      helpMsg = continueLineMsg;

    }

    helpTooltipElement.innerHTML = helpMsg;
    helpTooltip.setPosition(evt.coordinate);

    helpTooltipElement.classList.remove('hidden');
  };

  function addInteraction() {

    const type = 'LineString';
    draw = new Draw({
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

    props.map.addInteraction(draw);

    createMeasureTooltip();
    createHelpTooltip();

    let listener;
    draw.on('drawstart', function (evt) {
      // set sketch
      sketch = evt.feature;

      let tooltipCoord = evt.coordinate;

      listener = sketch.getGeometry().on('change', function (evt) {
        const geom = evt.target;
        let output;
        if (geom instanceof LineString) {
          output = formatLength(geom);
          tooltipCoord = geom.getLastCoordinate();
        }
        measureTooltipElement.innerHTML = output;
        measureTooltip.setPosition(tooltipCoord);
      });
    });

    draw.on('drawend', function () {
      measureTooltipElement.className = 'ol-tooltip ol-tooltip-static';
      measureTooltip.setOffset([0, -7]);
      // unset sketch
      sketch = null;
      // unset tooltip so that a new one can be created
      measureTooltipElement = null;
      createMeasureTooltip();
      unByKey(listener);
    });
  }

  function createHelpTooltip() {
    if (helpTooltipElement) {
      helpTooltipElement.parentNode.removeChild(helpTooltipElement);
    }
    helpTooltipElement = document.createElement('div');
    helpTooltipElement.className = 'ol-tooltip hidden';
    helpTooltip = new Overlay({
      element: helpTooltipElement,
      offset: [15, 0],
      positioning: 'center-left',
    });
    props.map.addOverlay(helpTooltip);
  }

  function createMeasureTooltip() {
    if (measureTooltipElement) {
      measureTooltipElement.parentNode.removeChild(measureTooltipElement);
    }
    measureTooltipElement = document.createElement('div');
    measureTooltipElement.className = 'ol-tooltip ol-tooltip-measure';
    measureTooltip = new Overlay({
      element: measureTooltipElement,
      offset: [0, -15],
      positioning: 'bottom-center',
      stopEvent: false,
      insertFirst: false,
    });
    props.map.addOverlay(measureTooltip);
  }

  const handleActivity = (activate) => {
    if (activate) {
      helpMsg = initMsg;
      props.map.on('pointermove', pointerMoveHandler);

      props.map.getViewport().addEventListener('mouseout', function () {
        helpTooltipElement.classList.add('hidden');
      });

      addInteraction();
      setActive(true);
    }
    else {
      props.map.removeInteraction(draw);
      helpMsg = '';
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
      <input type='image' alt='Mensurar distância em km.' src={clientWidth > 500 ? distanceMeasurer : distanceMeasurer_min} title='Mensurar distância.' onClick={handleClick} ref={htmlElement} className={"navbar-toggler DistanceMeasurer " + (active ? "DistanceMeasurerActive" : "DistanceMeasurerDeactive")}></input>
    </>
  )
}

export default DistanceMeasurer;