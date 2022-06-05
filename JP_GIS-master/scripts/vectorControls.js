VectorControls = {
handleMeasurements:null,
drawner:null,
modifyer:null,
drager:null,
hoverFeature:null,
selectedFeature:null,
printProvider:null,
printButton:null,
box:'',

createPanel:function (){
	
	this.box = new OpenLayers.Control.DrawFeature(VectorManager.vector,
			OpenLayers.Handler.RegularPolygon, {
        		displayClass: "olControlDrawGeoCodBox",
        		 title: "Caixa de Informação",
				handlerOptions : {
					sides : 4,
					snapAngle : 90,
					irregular : true,
					//persist : true
				}
			});
	
	this.box.handler.callbacks.done = VectorAction.drawGeocodBox;

	this.drager = new OpenLayers.Control.Navigation({title:'Arrastar Mapa', displayClass: 'olControlPanMap'});
	
	VectorManager.panel = new OpenLayers.Control.Panel({ defaultControl: this.drager });
	
	zoomToContextExtent = new OpenLayers.Control.Button({
        title: "Zoom Global", displayClass: "olControlZoomToMaxExtent", trigger: function(){ VectorManager.map.zoomToExtent(VectorManager.bounds); }
	});
	
	optionsLine = {
            handlerOptions: {
                persist: true
            },
            displayClass: "olControlMeasureDistance",
            title: "Medir Distância"
        };

	optionsPolygon = {
            handlerOptions: {
                persist: true
            },
            displayClass: "olControlMeasureArea",
            title: "Medir Área"
        };

	measureControls = {
            line: new OpenLayers.Control.Measure(
              OpenLayers.Handler.Path, 
              optionsLine 
            ),
            polygon: new OpenLayers.Control.Measure(
                OpenLayers.Handler.Polygon, 
                optionsPolygon
            )
        };
        
	for(var key in measureControls) {
            control = measureControls[key];
            control.events.on({
                "measure": handleMeasurements,
                "measurepartial": handleMeasurements
            });
    }   
	            
	function handleMeasurements(event) {
	            var geometry = event.geometry;
	            var units = event.units;
	            var order = event.order;
	            var measure = event.measure;
	            var element = document.getElementById('output');
	            if(order == 1){
	            	document.getElementById("mousePosition").innerHTML = ('Distância:'+measure.toFixed(3) + " " + units);
	            }
	            else{
	            	document.getElementById("mousePosition").innerHTML = ('Área:'+measure.toFixed(3) + " " + units+"<sup>2</sup>");
	            }
	 }
	
	 VectorManager.showPopUp = new OpenLayers.Control({
	        displayClass: "olControlFeatureInfo",
	        title: "Informação por Click"
	    });
	    
	    // register events to the featureInfo tool
	 VectorManager.showPopUp.events.register("activate", VectorManager.showPopUp, function() { VectorAction.togglePopUpMode(); });                
	 VectorManager.showPopUp.events.register("deactivate", VectorManager.showPopUp, function() { VectorAction.togglePopUpMode(); });

	 var geoLocation = new OpenLayers.Control.Button({
	        title: "Posição Corrente", displayClass: "olControlLocation", trigger: function(){ 
	        	
	        	if (navigator.geolocation) {
	        	  navigator.geolocation.getCurrentPosition(showPosition);
	        	} else {
	        		alert("Geolocation is not supported by this browser.");
	        	}
	        	

	        }
		});
	 
	VectorManager.panel.addControls([
		                              this.drager,
	  								  new OpenLayers.Control.ZoomBox({ title: "Zoom +" }),
		                              new OpenLayers.Control.ZoomBox({title:"Zoom -", displayClass: 'olControlZoomOutBox', out: true}),
		                              this.box,
		                              VectorManager.showPopUp,
		                              zoomToContextExtent,
		                              measureControls.line,
		      	                      measureControls.polygon, 
		      	                      geoLocation
		      	                   ]);	 
	
}

};

function showPosition  (position) {
	
	VectorManager.vectorFeatures.removeAllFeatures();
	
	var latLong = new OpenLayers.LonLat(position.coords.longitude, position.coords.latitude);
	
	console.log(position.coords.longitude +" "+position.coords.latitude)
	
	latLong = latLong.transform( new OpenLayers.Projection("EPSG:4326"), new OpenLayers.Projection("EPSG:900913"));
	
	VectorManager.map.setCenter(new OpenLayers.LonLat(latLong.lon, latLong.lat), 16);
	
	var point = new OpenLayers.Geometry.Point(latLong.lon, latLong.lat);
	
	/*var marker = new OpenLayers.Feature.Vector(point, null, {
	        externalGraphic: "/JP_GIS/images/location.png",
	        graphicWidth: 32,
	        graphicHeight: 32,
	        fillOpacity: 1
	    });*/
	
	var marker = new OpenLayers.Feature.Vector(point);
	
	VectorManager.vectorFeatures.addFeatures(marker);
	 
}