OpenLayers.Control.LayerSwitcher = OpenLayers.Class(OpenLayers.Control, {
	roundedCorner : true,
	roundedCornerColor : "darkblue",
	layerStates : null,
	layersDiv : null,
	baseLayersDiv : null,
	baseLayers : null,
	dataLbl : null,
	dataLayersDiv : null,
	dataLayers : null,
	minimizeDiv : null,
	maximizeDiv : null,
	ascending : true,
	initialize : function(options) {
		OpenLayers.Control.prototype.initialize.apply(this, arguments);
		this.layerStates = [];
	},
	destroy : function() {
		OpenLayers.Event.stopObservingElement(this.div);
		OpenLayers.Event.stopObservingElement(this.minimizeDiv);
		OpenLayers.Event.stopObservingElement(this.maximizeDiv);
		this.clearLayersArray("base");
		this.clearLayersArray("data");
		this.map.events.un({
			"addlayer" : this.redraw,
			"changelayer" : this.redraw,
			"removelayer" : this.redraw,
			"changebaselayer" : this.redraw,
			scope : this
		});
		OpenLayers.Control.prototype.destroy.apply(this, arguments);
	},
	setMap : function(map) {
		OpenLayers.Control.prototype.setMap.apply(this, arguments);
		this.map.events.on({
			"addlayer" : this.redraw,
			"changelayer" : this.redraw,
			"removelayer" : this.redraw,
			"changebaselayer" : this.redraw,
			scope : this
		});
	},
	draw : function() {
		OpenLayers.Control.prototype.draw.apply(this);
		this.loadContents();
		if (!this.outsideViewport) {
			this.minimizeControl();
		}
		this.redraw();
		return this.div;
	},
	clearLayersArray : function(layersType) {
		var layers = this[layersType + "Layers"];
		if (layers) {
			for ( var i = 0, len = layers.length; i < len; i++) {
				var layer = layers[i];
				OpenLayers.Event.stopObservingElement(layer.inputElem);
				OpenLayers.Event.stopObservingElement(layer.labelSpan);
			}
		}
		this[layersType + "LayersDiv"].innerHTML = "";
		this[layersType + "Layers"] = [];
	},
	checkRedraw : function() {
		var redraw = false;
		if (!this.layerStates.length
				|| (this.map.layers.length != this.layerStates.length)) {
			redraw = true;
		} else {
			for ( var i = 0, len = this.layerStates.length; i < len; i++) {
				var layerState = this.layerStates[i];
				var layer = this.map.layers[i];
				if ((layerState.name != layer.name)
						|| (layerState.inRange != layer.inRange)
						|| (layerState.id != layer.id)
						|| (layerState.visibility != layer.visibility)) {
					redraw = true;
					break;
				}
			}
		}
		return redraw;
	},
	redraw : function() {
		if (!this.checkRedraw()) {
			return this.div;
		}
		this.clearLayersArray("base");
		this.clearLayersArray("data");
		var containsOverlays = false;
		var containsBaseLayers = false;
		var len = this.map.layers.length;
		this.layerStates = new Array(len);
		for ( var i = 0; i < len; i++) {
			var layer = this.map.layers[i];
			this.layerStates[i] = {
				'name' : layer.name,
				'visibility' : layer.visibility,
				'inRange' : layer.inRange,
				'id' : layer.id
			};
		}
		var layers = this.map.layers.slice();
		if (!this.ascending) {
			layers.reverse();
		}
		for ( var i = 0, len = layers.length; i < len; i++) {
			var layer = layers[i];
			var baseLayer = layer.isBaseLayer;
			if (layer.displayInLayerSwitcher) {
				if (baseLayer) {
					containsBaseLayers = true;
				} else {
					containsOverlays = true;
				}
				var checked = (baseLayer) ? (layer == this.map.baseLayer)
						: layer.getVisibility();
				var inputElem = document.createElement("input");
				inputElem.id = this.id + "_input_" + layer.name;
				inputElem.name = (baseLayer) ? this.id + "_baseLayers"
						: layer.name;
				inputElem.type = (baseLayer) ? "radio" : "checkbox";
				inputElem.value = layer.name;
				inputElem.checked = checked;
				inputElem.defaultChecked = checked;
				if (!baseLayer && !layer.inRange) {
					inputElem.disabled = true;
				}
				var context = {
					'inputElem' : inputElem,
					'layer' : layer,
					'layerSwitcher' : this
				};
				OpenLayers.Event.observe(inputElem, "mouseup",
						OpenLayers.Function.bindAsEventListener(
								this.onInputClick, context));
				var labelSpan = document.createElement("span");
				OpenLayers.Element.addClass(labelSpan, "labelSpan");
				if (!baseLayer && !layer.inRange) {
					labelSpan.style.color = "gray";
				}
				labelSpan.innerHTML = layer.name;
				labelSpan.style.verticalAlign = (baseLayer) ? "bottom"
						: "baseline";
				OpenLayers.Event.observe(labelSpan, "click",
						OpenLayers.Function.bindAsEventListener(
								this.onInputClick, context));
				var br = document.createElement("br");
				var groupArray = (baseLayer) ? this.baseLayers
						: this.dataLayers;
				groupArray.push({
					'layer' : layer,
					'inputElem' : inputElem,
					'labelSpan' : labelSpan
				});
				var groupDiv = (baseLayer) ? this.baseLayersDiv
						: this.dataLayersDiv;
				groupDiv.appendChild(inputElem);
				groupDiv.appendChild(labelSpan);
				groupDiv.appendChild(br);
			}
		}
		this.dataLbl.style.display = (containsOverlays) ? "" : "none";
		this.baseLbl.style.display = (containsBaseLayers) ? "" : "none";
		return this.div;
	},
	onInputClick : function(e) {
		if (!this.inputElem.disabled) {
			if (this.inputElem.type == "radio") {
				this.inputElem.checked = true;
				this.layer.map.setBaseLayer(this.layer);
			} else {
				this.inputElem.checked = !this.inputElem.checked;
				this.layerSwitcher.updateMap();
			}
		}
		OpenLayers.Event.stop(e);
	},
	onLayerClick : function(e) {
		this.updateMap();
	},
	updateMap : function() {
		for ( var i = 0, len = this.baseLayers.length; i < len; i++) {
			var layerEntry = this.baseLayers[i];
			if (layerEntry.inputElem.checked) {
				this.map.setBaseLayer(layerEntry.layer, false);
			}
		}
		for ( var i = 0, len = this.dataLayers.length; i < len; i++) {
			var layerEntry = this.dataLayers[i];
			layerEntry.layer.setVisibility(layerEntry.inputElem.checked);
		}
	},
	maximizeControl : function(e) {
		this.div.style.width = "";
		this.div.style.height = "";
		this.showControls(false);
		if (e != null) {
			OpenLayers.Event.stop(e);
		}
	},
	minimizeControl : function(e) {
		this.div.style.width = "0px";
		this.div.style.height = "0px";
		this.showControls(true);
		if (e != null) {
			OpenLayers.Event.stop(e);
		}
	},
	showControls : function(minimize) {
		this.maximizeDiv.style.display = minimize ? "" : "none";
		this.minimizeDiv.style.display = minimize ? "none" : "";
		this.layersDiv.style.display = minimize ? "none" : "";
	},
	loadContents : function() {
		OpenLayers.Event.observe(this.div, "mouseup", OpenLayers.Function
				.bindAsEventListener(this.mouseUp, this));
		OpenLayers.Event.observe(this.div, "click", this.ignoreEvent);
		OpenLayers.Event.observe(this.div, "mousedown", OpenLayers.Function
				.bindAsEventListener(this.mouseDown, this));
		OpenLayers.Event.observe(this.div, "dblclick", this.ignoreEvent);
		this.layersDiv = document.createElement("div");
		this.layersDiv.id = this.id + "_layersDiv";
		OpenLayers.Element.addClass(this.layersDiv, "layersDiv");
		this.baseLbl = document.createElement("div");
		this.baseLbl.innerHTML = OpenLayers.i18n("Base Layer");
		OpenLayers.Element.addClass(this.baseLbl, "baseLbl");
		this.baseLayersDiv = document.createElement("div");
		OpenLayers.Element.addClass(this.baseLayersDiv, "baseLayersDiv");
		this.dataLbl = document.createElement("div");
		this.dataLbl.innerHTML = OpenLayers.i18n("Camadas");
		OpenLayers.Element.addClass(this.dataLbl, "dataLbl");
		this.dataLayersDiv = document.createElement("div");
		OpenLayers.Element.addClass(this.dataLayersDiv, "dataLayersDiv");
		if (this.ascending) {
			this.layersDiv.appendChild(this.baseLbl);
			this.layersDiv.appendChild(this.baseLayersDiv);
			this.layersDiv.appendChild(this.dataLbl);
			this.layersDiv.appendChild(this.dataLayersDiv);
		} else {
			this.layersDiv.appendChild(this.dataLbl);
			this.layersDiv.appendChild(this.dataLayersDiv);
			this.layersDiv.appendChild(this.baseLbl);
			this.layersDiv.appendChild(this.baseLayersDiv);
		}
		this.div.appendChild(this.layersDiv);
		if (this.roundedCorner) {
			OpenLayers.Rico.Corner.round(this.div, {
				corners : "tl bl",
				bgColor : "transparent",
				color : this.roundedCornerColor,
				blend : false
			});
			OpenLayers.Rico.Corner.changeOpacity(this.layersDiv, 0.75);
		}
		var imgLocation = OpenLayers.Util.getImagesLocation();
		var sz = new OpenLayers.Size(18, 18);
		var img = imgLocation + 'layer-switcher-maximize.png';
		this.maximizeDiv = OpenLayers.Util.createAlphaImageDiv(
				"OpenLayers_Control_MaximizeDiv", null, sz, img, "absolute");
		OpenLayers.Element.addClass(this.maximizeDiv, "maximizeDiv");
		this.maximizeDiv.style.display = "none";
		OpenLayers.Event.observe(this.maximizeDiv, "click", OpenLayers.Function
				.bindAsEventListener(this.maximizeControl, this));
		this.div.appendChild(this.maximizeDiv);
		var img = imgLocation + 'layer-switcher-minimize.png';
		var sz = new OpenLayers.Size(18, 18);
		this.minimizeDiv = OpenLayers.Util.createAlphaImageDiv(
				"OpenLayers_Control_MinimizeDiv", null, sz, img, "absolute");
		OpenLayers.Element.addClass(this.minimizeDiv, "minimizeDiv");
		this.minimizeDiv.style.display = "none";
		OpenLayers.Event.observe(this.minimizeDiv, "click", OpenLayers.Function
				.bindAsEventListener(this.minimizeControl, this));
		this.div.appendChild(this.minimizeDiv);
	},
	ignoreEvent : function(evt) {
		OpenLayers.Event.stop(evt);
	},
	mouseDown : function(evt) {
		this.isMouseDown = true;
		this.ignoreEvent(evt);
	},
	mouseUp : function(evt) {
		if (this.isMouseDown) {
			this.isMouseDown = false;
			this.ignoreEvent(evt);
		}
	},
	CLASS_NAME : "OpenLayers.Control.LayerSwitcher"
});