import './PngExporter.css';
import React, { useRef } from 'react';
import pngExporter from '../../_assets/img/pngExporter.png';
import pngExporter_min from '../../_assets/img/pngExporter_min.png';

function PngExporter(props) {

  const htmlElement = useRef();

  const clientWidth = document.documentElement.clientWidth;

  const handleClick = () =>{
     // props.handleActiveTool(htmlElement);
      props.map.once('rendercomplete', function () {
      const mapCanvas = document.createElement('canvas');
      mapCanvas.crossorigin="anonymous"
      const size = props.map.getSize();
      mapCanvas.width = size[0];
      mapCanvas.height = size[1];
      const mapContext = mapCanvas.getContext('2d');
      Array.prototype.forEach.call(
        document.querySelectorAll('.ol-layer canvas'),
        function (canvas) {
          if (canvas.width > 0) {
            const opacity = canvas.parentNode.style.opacity;
            mapContext.globalAlpha = opacity === '' ? 1 : Number(opacity);
            const transform = canvas.style.transform;
            // Get the transform parameters from the style's transform matrix
            const matrix = transform
              .match(/^matrix\(([^\(]*)\)$/)[1]
              .split(',')
              .map(Number);
            // Apply the transform to the export map context
            CanvasRenderingContext2D.prototype.setTransform.apply(
              mapContext,
              matrix
            );
            mapContext.drawImage(canvas, 0, 0);
          }
        }
      );
      if (navigator.msSaveBlob) {
        // link download attribute does not work on MS browsers
        navigator.msSaveBlob(mapCanvas.msToBlob(), 'map.png');
      } else {
        const link = document.getElementById('image-download');
        link.href = mapCanvas.toDataURL();
        link.click();
      }
    });
    props.map.renderSync();
  }
 
  return (   
    <>
    <input type='image' alt='Capturar imagem' src={clientWidth>500?pngExporter:pngExporter_min} title='Capturar imagem.' onClick={handleClick} ref={htmlElement} className="navbar-toggler PngExporter"></input>  
    <a crossOrigin="anonymous" id='image-download' download="map.png" href='#'> </a>
    </>
  ) 

}

export default PngExporter;