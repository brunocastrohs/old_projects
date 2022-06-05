VectorAction = {
	
	featureCount:0,
	filterParams: [],
	legendOptions: {},

	toggleQueryMode : function() {
		if (VectorManager.featureInfo.active) {
			VectorManager.queryEventHandler.activate();
		} else {
			VectorManager.queryEventHandler.deactivate();
		}
	},
	
	getVisibleLayers : function() {

		var stg = '';

		for ( var h = 0; h != VectorManager.map.layers.length; h++) {

			if (VectorManager.map.layers[h].CLASS_NAME.indexOf('Vector') != -1
					&& VectorManager.map.layers[h].visibility == true) {
				stg += VectorManager.map.layers[h].options.SCHEMA + "."
						+ VectorManager.map.layers[h].params['LAYERS'] + ";";
			}
		}

		return stg;

	},
	
	setLayerVisibility:function(gid){
		VectorManager.vectorFeatures.removeAllFeatures();
		VectorManager.map.layers[gid].setVisibility(!VectorManager.map.layers[gid].visibility);
		var x = VectorManager.map.layers;
		x = 10;
	},
	
	drawGeocodBox:function(bbox){
		
		VectorManager.vectorFeatures.removeAllFeatures();
		
		var layers = VectorManager.map.layers;
	
		if (layers != null) {
			
			var x="",s="";
			var i = 0;
			//here
			for(i = 0; i != layers.length; i++){
				if(layers[i].visibility && layers[i].options.SCHEMA && layers[i].options.CONSULTABLE){
					x += (layers[i]?layers[i].params.LAYERS:"")+":";
					s += (layers[i]?layers[i].options.SCHEMA:"")+":";
				}
			}
			
			var bounds = bbox.getBounds();

			var feature = new OpenLayers.Feature.Vector(bounds.toGeometry());
		
			//VectorManager.vectorFeature.addFeatures(feature);
		
			var wkt = new OpenLayers.Format.WKT();
        
			var str = "SRID=900913;"+wkt.write(feature);
		
			var f = '';//VectorAction.currentFilter;
			
			var post = {
					action: "featureInfo",
					geocodbox: str,
					x:x,
					s:s,
					f:f
			};

			$.ajax({
	            
	            type: "POST",
	            data: post,             
	            url: "/JP_GIS/src/server/RequestListener.php",
	            dataType: "html",
	            success: function(result){
	              	
	            	Listener.openDialogWindow("Informações",result,[],false);
	            	
	            	//setTimeout(function(){$('#messageBox').hide();}, 2000);
	            	
	            },
	            error: function(result){
	              //  alert(result);
	               // alert("Never sends.");
	            },
	            beforeSend: function(){
	            	//$('#nav_container').hide();
	            	//$('#preloader').show();
	            },
	            complete: function(msg){
	            	//$('#nav_container').show();
	            	//$('#preloader').hide();
	            }
	        });
			
			}
			else{}

	}, 
	
	setCenter:function(coord){
		
		 if(coord != ''){
			var latLong = coord.split(' ');
			
			VectorManager.vectorFeatures.removeAllFeatures();
			$('#modalWindow').hide();
			location.hash = '#';
			if(VectorManager.projection == 'EPSG:900913')
				VectorManager.map.setCenter(new OpenLayers.LonLat(latLong[0], latLong[1]), 16);
			else
				VectorManager.map.setCenter(new OpenLayers.LonLat(latLong[0], latLong[1]), 5);

			VectorManager.vectorFeatures.addFeatures(new OpenLayers.Feature.Vector(new OpenLayers.Geometry.Point(latLong[0], latLong[1])));
		}
		 
	},

	setLegendFilter:function(field,element,id){
		
		var i = 0, hasFilterActivated = false, isFirst = true; 
		
		this.filterParams[''+id] = {
				cql_filter : ""
		};
		
		for(i=0; i < this.legendOptions[element.className].length; i++){
			var e =  document.getElementById(this.legendOptions[element.className][i]);
			if(e.checked && isFirst){
				isFirst = false;
				hasFilterActivated = true;
				this.filterParams[''+id]['cql_filter'] += field +" = '"+e.value+"'";
			}else if(e.checked){
				this.filterParams[''+id]['cql_filter'] += " OR " + field +" = '"+e.value+"'";
			}
			
		}
		
		if(!hasFilterActivated)
			this.filterParams[''+id]['cql_filter'] = null;
			
		VectorManager.map.layers[id].mergeNewParams(this.filterParams[''+id]);
		VectorManager.map.layers[id].redraw(true);
		
	},
	
	setQueryFilter:function(name, id){
		
		var isFirst = true;
		var v_key = (document.getElementsByName(name+'.Key')) ;
		var v_attrib = (document.getElementsByName(name+'.AttribDropdown'));
		var v_op = (document.getElementsByName(name+'.OperatorDropdown')) ;
		var v_connective = (document.getElementsByName(name+'.LogicDropdown')) ;
		
		var cqlString = null;
		
		for(let i = 0; i < v_key.length; i++){
				var key = v_key[i].value;
				var attrib = v_attrib[i].value.split("!!");
				var op = v_op[i].value;
				var connective = v_connective[i].value;
				if(attrib[1] && op && key!=''){
			
					if(isFirst){
						if(attrib[1].includes('char') || attrib[1].includes('text'))
							if(op == 'ilike')
								cqlString = attrib[0]+" "+op+" '%"+key+"%'";
							else
								cqlString = attrib[0]+" "+op+" '"+key+"'";
						else if(op != 'ilike') 
							cqlString = attrib[0]+" "+op+" "+key;
					}
					else
						if(attrib[1].includes('char') || attrib[1].includes('text'))
							if(op == 'ilike')
								cqlString += connective+" "+attrib[0]+" "+op+" '%"+key+"%'";
							else
								cqlString += connective+" "+attrib[0]+" "+op+" '"+key+"'";
						else if(op != 'ilike') 
							cqlString += connective+" "+attrib[0]+" "+op+" "+key;

					//parei aqui
					isFirst = (cqlString == null);
					
				}
			}
		
			
			if(cqlString != null){
				this.filterParams[''+id] = {
						cql_filter : cqlString
				};
					
				VectorManager.map.layers[id].mergeNewParams(this.filterParams[''+id]);
				VectorManager.map.layers[id].redraw(true);
			}
			else{
				this.filterParams[''+id] = {
						cql_filter : cqlString
				};
				VectorManager.map.layers[id].mergeNewParams(this.filterParams[''+id]);
				VectorManager.map.layers[id].redraw(true);
			}
		
	},
	
	resetQueryFilter:function(name, id){
		
		var v_key = (document.getElementsByName(name+'.Key')) ;
		var v_attrib = (document.getElementsByName(name+'.AttribDropdown'));
		var v_op = (document.getElementsByName(name+'.OperatorDropdown')) ;
		var v_connective = (document.getElementsByName(name+'.LogicDropdown')) ;
		
		for(let i = 0; i < v_key.length; i++){
		
			v_key[i].value = '';
			v_attrib[i].value = '';
			v_op[i].value = '';
			v_connective[i].value = '';
			
		}
		
		VectorAction.setQueryFilter(name, id);
		document.getElementById('multiFilter.'+name).innerHTML = '';
		
	},
	
	showMetaLegend:function(id, e){
		var x = document.getElementById(id+"LGS");
		if(x.value == "1"){
			x.value = "0";
			e.innerHTML = "Esconder Legenda";
			document.getElementById(id+"LG").style.display = "block"; 
		}
		else{
			x.value = "1";
			e.innerHTML = "Visualizar Legenda";
			document.getElementById(id+"LG").style.display = "none";
		}
	},
	
	showMetaLegend:function(id, e){
		var x = document.getElementById(id+"LGS");
		if(x.value == "1"){
			x.value = "0";
			e.innerHTML = "Esconder Legenda";
			document.getElementById(id+"LG").style.display = "block"; 
		}
		else{
			x.value = "1";
			e.innerHTML = "Visualizar Legenda";
			document.getElementById(id+"LG").style.display = "none";
		}
	},
	
	showMetaDescription:function(id, e){
		var x = document.getElementById(id+"DSS");
		if(x.value == "1"){
			x.value = "0";
			e.innerHTML = "Esconder Descrição";
			document.getElementById(id+"DS").style.display = "block";
		}
		else{
			x.value = "1";
			e.innerHTML = "Visualizar Descrição";
			document.getElementById(id+"DS").style.display = "none";
		}
	},
	
	showMetaQuery:function(id, e){
		var x = document.getElementById(id+"QSS");
		if(x.value == "1"){
			x.value = "0";
			e.innerHTML = "Esconder Pesquisador";
			document.getElementById(id+"QS").style.display = "block";
		}
		else{
			x.value = "1";
			e.innerHTML = "Visualizar Pesquisador";
			document.getElementById(id+"QS").style.display = "none";
		}
	},
	
	pushNewFilter:function(name, id){
		
		var br = document.createElement('br');
		
		var hr = document.createElement('hr');
		
		var logical = document.getElementById(name+'.LogicDropdownContainer').cloneNode(true);
		
		logical.style.display = 'block';
		
		document.getElementById('multiFilter.'+name).appendChild(hr);
		document.getElementById('multiFilter.'+name).appendChild(logical);
		document.getElementById('multiFilter.'+name).appendChild(br.cloneNode(true));
		document.getElementById('multiFilter.'+name).appendChild(document.getElementById(name+'.AttribDropdown').cloneNode(true));
		document.getElementById('multiFilter.'+name).appendChild(br.cloneNode(true));
		document.getElementById('multiFilter.'+name).appendChild(document.getElementById(name+'.OperatorDropdown').cloneNode(true));
		document.getElementById('multiFilter.'+name).appendChild(br.cloneNode(true));
		document.getElementById('multiFilter.'+name).appendChild(document.getElementById(name+'.Key').cloneNode(true));
		document.getElementById('multiFilter.'+name).appendChild(br.cloneNode(true));
		
	},
	
	dropNewFilter:function(name, id){
		document.getElementById('multiFilter.'+name).innerHTML = '';
		
	},
	
	exportData:function(name, id){
		//console.log(document.getElementById(name));
		//console.log(VectorManager.map.layers[id].SCHEMA);
		if(VectorAction.filterParams[''+id] && VectorAction.filterParams[''+id].cql_filter != '' && VectorAction.filterParams[''+id].cql_filter != null )
			window.open('/JP_GIS/src/server/RequestListener.php?x=exportData&y='+VectorManager.map.layers[id].SCHEMA+'&z='+name+'&k='+VectorAction.filterParams[''+id].cql_filter);
	},
	
	autoComplete:function(id, input){

		let operatorCombobox = input.parentElement.previousSibling.previousSibling.firstChild.firstChild;
		let attribCombobox = operatorCombobox.parentElement.parentElement.previousSibling.previousSibling.firstChild.firstChild;
		
		if(attribCombobox.options[attribCombobox.selectedIndex].value != ""){
			let attribData = attribCombobox.options[attribCombobox.selectedIndex].value.split("!!");
			let attrib = attribData[0];
			let type = attribData[1].includes('char') || attribData.includes('text');
			if(input.value.length >= 3 && type){
				let x = VectorManager.map.layers[id].SCHEMA;
				let y = input.id.split(".")[0];
				let w = attrib;
				let z = input.value;
				Listener.sendAutoCompleteRequest(input.nextSibling, x, y, w, z);
			}else
				input.nextSibling.innerHTML = '';
		}
		
		
	},
	
	selectAutoCompleteItem:function(item){

		let value = item.innerHTML;
		
		item.parentElement.parentElement.previousSibling.value = value;//.value = value;
		
		item.parentElement.innerHTML = '';
		
	},
	
	/************** on click *****************************/
	
	
	openPopUp : function(evt) {

		VectorManager.vectorFeatures.removeAllFeatures();
		
		var layers = VectorManager.map.layers;
	
		if (layers != null && evt) {
			
			var x="",s="";
			var i = 0;
			//here
			for(i = 0; i != layers.length; i++){
				if(layers[i].visibility && layers[i].options.SCHEMA && layers[i].options.CONSULTABLE){
					x += (layers[i]?layers[i].params.LAYERS:"")+":";
					s += (layers[i]?layers[i].options.SCHEMA:"")+":";
				}
			}
			
			var lonLat = VectorManager.map.getLonLatFromPixel(evt.xy);
			
			var post = {
					action: "popUpInfo",
					x:x,
					s:s,
					lon:lonLat.lon,
					lat:lonLat.lat
			};

			$.ajax({
	            
	            type: "POST",
	            data: post,             
	            url: "/JP_GIS/src/server/RequestListener.php",
	            dataType: "html",
	            success: function(result){
	              	
	            	Listener.openDialogWindow("Informações",result,[],false);
	            	
	            	//setTimeout(function(){$('#messageBox').hide();}, 2000);
	            	
	            },
	            error: function(result){
	              //  alert(result);
	               // alert("Never sends.");
	            },
	            beforeSend: function(){
	            	//$('#nav_container').hide();
	            	//$('#preloader').show();
	            },
	            complete: function(msg){
	            	//$('#nav_container').show();
	            	//$('#preloader').hide();
	            }
	        });
			
			}
			else{}
		
	},

	togglePopUpMode : function() {
		if (VectorManager.showPopUp.active) {
			VectorManager.popUpEventHandler.activate();
		} else {
			VectorManager.popUpEventHandler.deactivate();
		}
	},

	
};