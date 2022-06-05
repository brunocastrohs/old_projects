VectorManager = {
	map : '',
	containers:'',
	layers: [],
	vector : null,
	vectorFeatures : null,
	selectLayers : '',
	selectedLayer : '',
	bounds : null,
	extent: null,
	zoom : null,
	center : null,
	projection : null,
	gmap : null,
	ghyb : null,
	webLayers: null,
	layersLoaded : '',
	panel : '',
	ovLayers : null, featureInfo : '', queryEventHandler : '', popUpEventHandler : '', showPopUp : '', 
	layerId : '',
	vectorCount : 1,
	initialFeatures : [],
	user : 1,
	isInsert : false,
	uId : false,
	hasD : false,
	hasU : false,
	hasS : false,
	initOptions : function() {

		var options = null;
		
		var options = {
			//restrictedExtent: this.extent,
			allOverlayes : true,
			allOverlays : true,
			controls : [],
			maxExtent : this.bounds,
			projection : this.projection,
			units : 'm',
			maxResolution : 500,
			id : "vectorMap"
		};

		this.webLayers = [
			new OpenLayers.Layer.OSM('Open Street Maps (OSM)',null, {visibility:true})
		];

		return options;
		
	},
	initLayers : function() {

		VectorStyle.init();

		this.map.addLayers(this.webLayers);
		
		for(i = 0; i != this.layers.length; i++){
			
			this.map.addLayer(this.returnOlLayer(this.layers[i]));
				
		}
		
		this.vector = new OpenLayers.Layer.Vector(
				"Vetores",
				{
					styleMap : VectorStyle.vectorStyles
				});
		
		this.vectorFeatures = new OpenLayers.Layer.Vector("Vetores de Pesquisa");
		
		this.map.addLayer(this.vector);
		this.map.addLayer(this.vectorFeatures);
		
		
		var boundings = new OpenLayers.Bounds(
	                -40.6008702709999, -7.85746481299992,
	                -38.5362076389999, -6.76291935799992
	        );
		  
		
	},
	returnOlLayer:function(layer){
		
		return new OpenLayers.Layer.WMS(
				layer['title'], 
				layer['url'],
				{'layers':layer['name'],transparent: true, format: 'image/png'},
				{transitionEffect:'resize', displayInLayerSwitcher: layer['layerswitcher'] , VIEW:layer['view'], GROUP:layer['group'],CONSULTABLE:layer['consultable'],SCHEMA:layer['schema'],CONTAINER:layer['container'],GID:layer['gid'],visibility:layer['visibility'], singleTile: true}
				);
		
	},
	initMap : function() {

		this.map = new OpenLayers.Map('vectorMap', this.initOptions());

		this.initLayers();

	},

	initGIS : function() {

		document.getElementById('vectorMap').style.height = (window.innerHeight-document.getElementById('nav').clientHeight)+'px';
		
		this.initMap();

		this.map.addControl(new OpenLayers.Control.ScaleLine());
		
		VectorControls.createPanel();

		this.map.addControl(this.panel);

		VectorManager.popUpEventHandler = new OpenLayers.Handler.Click({ 'map': this.map }, { 'click': function(e) { VectorAction.openPopUp(e); } });		    
		
		this.map.events.register('mousemove', this.map, function(e) {
			var lonLat = VectorManager.map.getLonLatFromPixel(e.xy);
			lonLat = lonLat.transform(new OpenLayers.Projection("EPSG:900913"),
					new OpenLayers.Projection("EPSG:4326"));
			document.getElementById("mousePosition").innerHTML = (
					'Lon: ' + lonLat.lon + ' , Lat: ' + lonLat.lat);
		});
		
		$(document).mouseup(function(e) {

			var container = $(".overlay");
		    
			if (!(!container.is(e.target) && container.has(e.target).length === 0)) {
				window.location.href ="#";
		    }
		    
		});
		
		OpenLayers.Event.observe(document, "keydown", function(evt) {
			var handled = false;
			switch (evt.keyCode) {
			case 27: // esc
				window.location.href ="#";
				handled = true;
				break;
			}
			if (handled) {
				OpenLayers.Event.stop(evt);
			}
		});

		this.map.setCenter(new OpenLayers.LonLat(this.center[0],this.center[1]), this.zoom);

	}

};
