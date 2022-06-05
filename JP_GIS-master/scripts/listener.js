Listener = {
	
	x:"",	
	y:"",
	z:"",
	k:"",
	w:false,
	idGIS:false,
	currentRow:false,
	rowCount:0,
	multiForm:[],
	isDateQuery:false,
	fV:false,
	fC:false,
	isInfo:false,
	
	getFile:function(file){
		
		window.open(file,'_blank');
		
	},
	
	sendFormRequest:function(id){
		
		if(id){
			window.location = "/JP_GIS/index.php?x=formCRUD&y="+Util['y']+"&z="+Util['z']+"&k="+Util['k']+"&w="+id;
		}else if(Util['fC'] && Util['fV']){
			window.location = "/JP_GIS/index.php?x=formCRUD&y="+Util['y']+"&z="+Util['z']+"&k="+Util['k']+"&fC="+Util['fC']+"&fV="+Util['fV'];
		}else
			window.location = "/JP_GIS/index.php?x=formCRUD&y="+Util['y']+"&z="+Util['z']+"&k="+Util['k'];
		
	},
	
	login:function(){
		
		var serializedData=this.toJson('adminForm');
		
		serializedData['action'] = "loginAdmin";
		
		$.ajax({
            
            type: "POST",
            data: serializedData,             
            url: "/JP_GIS/src/server/RequestListener.php",
            dataType: "html",
            success: function(result){
                
            	if(result==1){
            		window.location = "/JP_GIS/index.php";
            	}
            	else{
            		alert('Usuário ou senha incorretos. Tente novamente.');
            	}
            	
            },
            error: function(result){
                //$("#bla").html('');
                alert("Never happens.");
            },
            beforeSend: function(){
              //  $('#loading').css({display:"block"});
            },
            complete: function(msg){
              //  $('#loading').css({display:"none"});
            }
        });
		
	},	
	
	sendFileRequest:function(button){
		
	    var serializedData = new FormData($('form')[0]);
	    	
		$.ajax({
            
            type: "POST",
            data: serializedData,             
            url: "/JP_GIS/src/server/RequestListener.php",
            cache: false,
            contentType: false,
            processData: false,
            success: function(result){
              	
            	$('#messageBox').html(result);
            	$('#messageBox').show();
            	
            	setTimeout(function(){$('#messageBox').hide();}, 2000);
            	
              	setTimeout(function() {window.location = '/JP_GIS/index.php?x=homePageCRUD&y='+Util['y']+'&z='+Util['z'];}, 3000);		
              	
            },
            error: function(result){
                alert("Never sends.");
            },
            beforeSend: function(){
            	button.disabled = true;
            	$('#nav_container').hide();
            	$('#preloader').show();
            },
            complete: function(msg){
            	$('#nav_container').show();
            	$('#preloader').hide();
            	button.disabled = false;
            }
        });
		
	},
	
	sendRequest:function(action,w,button){
		
		if(Listener.idGIS){
	    	w = Listener.idGIS;
	    }
		
	    var serializedData = {action:action,y:Util['y'],z:Util['z'],k:Util['k'],w:w};
		 
	    if(document.getElementById('crudFORM'))
	    	serializedData['data']=this.toJson('crudFORM');
	    
		$.ajax({
            
            type: "POST",
            data: serializedData,             
            url: "/JP_GIS/src/server/RequestListener.php",
            dataType: "html",
            success: function(result){
              	
            	$('#messageBox').html(result);
            	$('#messageBox').show();
            	
            	setTimeout(function(){$('#messageBox').hide();}, 2000);
            	
            	if(action=="remove" && document.getElementById('tableCRUD')){
            		Listener.removeTableRow();
             	}else if(document.getElementById('gisForm')){

             		VectorControls.selectedFeature.attributes = serializedData['data'];
             		
             		VectorControls.hoverFeature.unselectAll();
             		
             	}else{
             		setTimeout(function() {history.back();}, 3000);		
             	}   
              	
            },
            error: function(result){
                //$("#bla").html('');
                alert("Never sends.");
            },
            beforeSend: function(){
            	//button.disabled = true;
            	$('#nav_container').hide();
            	$('#preloader').show();
            },
            complete: function(msg){
            	$('#nav_container').show();
            	$('#preloader').hide();
            	//button.disabled = false;
            }
        });
		
	},
	
	sendMultiFormRequest:function(action,w,button){
		
	    var serializedData = {action:action,w:w,x:""};
		
	    var isFirst = true;
	    
	    jQuery.each(Listener.multiForm, function(key,data) {
	    	data  = data.split(":");
	    	serializedData[data[0]+"."+data[1]] = {};
	    	serializedData[data[0]+"."+data[1]]['k'] = data[2];
	    	serializedData[data[0]+"."+data[1]]['data']=Listener.toJsonFromMultiForm('crudFORM.'+data[1]);
	    	serializedData['x'] += data[0]+"."+data[1]+";";
	    	if(isFirst){
	    		Util['y'] = data[0];
	    		Util['z'] = data[1];
	    	}
	    	isFirst = false;
	    });
	    	
		$.ajax({
            
            type: "POST",
            data: serializedData,             
            url: "/JP_GIS/src/server/RequestListener.php",
            dataType: "html",
            success: function(result){
            	$('#messageBox').html(result);
            	$('#messageBox').show();
            	setTimeout(function() {$('#messageBox').hide();}, 2000);
         		setTimeout(function() {history.back();}, 3000);		
            },
            error: function(result){
               alert("Never sends.");
            },
            beforeSend: function(){
            	button.disabled = true;
            	$('#nav_container').hide();
            	$('#preloader').show();
            },
            complete: function(msg){
            	$('#nav_container').show();
            	$('#preloader').hide();
            	button.disabled = false;
            }
            
        });
		
	},
	
	sendQuery:function(){
		
		var serializedData = {action:"queryData",y:Util['y'],z:Util['z'],k:Util['k']};
		
		if(Util['fC'] && Util['fV']){
			serializedData['fC'] = Util['fC'];
			serializedData['fV'] = Util['fV'];
		}
		
		if(Util['isDateQuery']){
			Listener.makeDateKeyValue();
		}
		
		if(Util['isInfo']){
			serializedData['isInfo'] = Util['z'];
		}
		
		serializedData['data']=this.toJson('homePageForm');
			
		$.ajax({
            
            type: "POST",
            data: serializedData,             
            url: "/JP_GIS/src/server/RequestListener.php",
            dataType: "html",
            success: function(result){
            	
            	$('#tableCRUDBody').html(result);
            	
            },
            error: function(result){
                //$("#bla").html('');
                alert("Never happens.");
            },
            beforeSend: function(){
              //  $('#loading').css({display:"block"});
            },
            complete: function(msg){
              //  $('#loading').css({display:"none"});
            }
        });
		
		Util['isDateQuery'] = false;
		
	},
	
	getRowIndex:function(r){
		
		Util['currentRow'] = r;
	 	
	},
	
	removeTableRow:function(){
		
		$('#crudRow'+Util['currentRow']).html("");
		Util['rowCount']--;
		$('#rowCounter').html(Util['rowCount']+ " Registros");
	},
	
	logout:function(){
		 // var serializedData = $('#adminForm').serialize();
		
		$.ajax({
            
            type: "POST",
            data: {action:'logout'},             
            url: "/JP_GIS/src/server/RequestListener.php",
            dataType: "html",
            success: function(result){
                //$("#bla").html('');
               // alert(result);
            	if(result==1){
            		window.location = "/JP_GIS/index.php";
            	}
            	else{
            		alert('Login falhou...');
            	}
            },
            error: function(result){
                //$("#bla").html('');
                alert("Never happens.");
            },
            beforeSend: function(){
              //  $('#loading').css({display:"block"});
            },
            complete: function(msg){
              //  $('#loading').css({display:"none"});
            }
        });
		
	},

	toJson:function(id){

		var array = $('#'+id).serializeArray();
	    
		var json = {};
	    
	    jQuery.each(array, function() {
	        json[this.name] = this.value || '';
	    });
	    
	    if(document.getElementById('gisForm') && json['tipo'].indexOf('1')!=-1){
	    	json['id_obra'] = '';
	    }
	    else if(document.getElementById('gisForm') && json['tipo'].indexOf('2')!=-1){
	    	json['id_projeto'] = '';
	    }
	    
	    return json;
	    
	},
	
	toJsonFromMultiForm:function(id){

		var data = document.forms[id].getElementsByTagName("input");
		
		var json = {};
	    
	    for(i =0; i != data.length; i++){
	    	if(data[i].type=='radio' && data[i].checked)
	    		json[data[i].name] = data[i].value;
	    	else if(data[i].type!='radio')
	    		json[data[i].name] = data[i].value;
	    }
	    
	    var data = document.forms[id].getElementsByTagName("select");
		
	    for(i =0; i != data.length; i++){
	    	if(data[i].value)
	    		json[data[i].name] = data[i].value;
	    }
	    
	    var data = document.forms[id].getElementsByTagName("textarea");
		
	    for(i =0; i != data.length; i++){
	    	json[data[i].name] = data[i].value;
	    }
	    
	    return json;
	    
	},
	
	openDialogWindow:function(title,content,buttons,hasIFRAME){

	    var toolBar = "<a href='#' onclick='window.location.href =\"#\";' class='btn'>Fechar</a>";
		
	    if(buttons && buttons[0])
	    	for(i = 0; i != buttons.length; i++){
	    		toolBar +=  "<a href='#' onclick='"+buttons[i]['action']+"' class='btn'>"+buttons[i]['text']+"</a>";;
	    	}
	    
	    $('#modalWindowButtons').html(toolBar);
	    
	    content = hasIFRAME?"<iframe src='"+content+"' id='ipage' style=''></iframe>":content;
	    
	    $('#modalWindowContent').html(content);
	    
	    $('#modalWindowTitle').html(title);
	    
	    location.hash = '#modalWindow';
	    
	    $("#menu").hide();
	    
	    $("#menu").hide();
	   // $("#modalWindow").focus();
	    $("#modalWindow").show();
	    
	    
	    return true;
	        
	},
	
	closeDialogWindow:function(){

	    alert('jhon');
	    $("#modalWindow").hide();
	    
	    return true;
	        
	},
	
	changeSearchKey:function(value){

		var data = value.split(".");
		var type = data[1];
		var field = data[0];
		
		if(type.indexOf('time')!=-1 || type.indexOf('date')!=-1){
	    	$('#searchBar').html("<input type='text'  name='searchKey1' id='searchKey1' placeholder='Data Inicial - 00/00/0000' />" +
	    						 "<input type='text'  name='searchKey2' id='searchKey2' placeholder='Data Final - 00/00/0000' />" +
	    						 "<input type='text'  name='searchKey'  id='searchKey' style='display:none;'/>");
	    	Listener.isDateQuery = true;
	    }else if(type.indexOf('fk')!=-1){
	    	Listener.sendForeignQueryAttributesRequest(field);
	    }else{
	    	$('#searchBar').html("<input type='text'  name='searchKey' id='searchKey' placeholder='Valor de Pesquisa...' />");
	    }
	    
	},
	
	sendAutoCompleteRequest:function(autoList, x, y, w, z){
		
	    var serializedData = {action:"autoComplete", x:x, y:y, w:w, z:z};
			
		$.ajax({
            
            type: "POST",
            data: serializedData,             
            url: "/JP_GIS/src/server/RequestListener.php",
            dataType: "html",
            success: function(result){
              	
            	autoList.innerHTML = result;  
              	
            },
            error: function(result){
                alert("Sem conexão com o servidor.");
            },
            beforeSend: function(){
            	autoList.innerHTML = 'Carregando sugestões...'
            	//$('#nav_container').hide();
            	//$('#preloader').show();
            },
            complete: function(msg){
            	//autoList.innerHTML = ''
            	//$('#nav_container').show();
            	//$('#preloader').hide();
            }
        });
		
	},
	
	makeDateKeyValue:function(){
		
		var value = " between '"+document.getElementById('searchKey1').value+"' and '"+document.getElementById('searchKey2').value+"'";
		//value = " between '"+$('#searchKey1').val()+"' and '"+$('#searchKey2').val()+"'";
		
		document.getElementById('searchKey').value = value;
		//$('#searchKey').val(value);
	    
		value = '';
	},
	//generateForeignQueryAttributes
	sendForeignQueryAttributesRequest:function(field){
		
	    var serializedData = {action:"foreignQueryAttributes",y:Util['y'],z:Util['z'],k:field};
			
		$.ajax({
            
            type: "POST",
            data: serializedData,             
            url: "/JP_GIS/src/server/RequestListener.php",
            dataType: "html",
            success: function(result){
              	
            	$('#searchBar').html(result);  
              	
            },
            error: function(result){
                alert("Never sends.");
            },
            beforeSend: function(){
            	$('#nav_container').hide();
            	$('#preloader').show();
            },
            complete: function(msg){
            	$('#nav_container').show();
            	$('#preloader').hide();
            }
        });
		
	},
	
	collapse:function(id,div){
		
		if(document.getElementById(id).style.display == 'none'){
			document.getElementById(id).style.display = 'inline';
			div.style.backgroundImage = 'url(/JP_GIS/images/collapse.gif)';

			//url('/JP_GIS/images/collapse.gif')
		}
		else{
			document.getElementById(id).style.display = 'none';
			div.style.backgroundImage = 'url(/JP_GIS/images/expand.gif)';

		}
		
	},
	
	collapseBranch:function(id){
		
		id = "#"+id;
		var idT = id+"T";
		var cond = $(id).is(':visible');
		if(cond){
			$(id).hide();
			$(idT).css('background-image', "url('/JP_GIS/images/open_branch.png')");
		}
		else{
			$(id).show();
			$(idT).css('background-image', "url('/JP_GIS/images/close_branch.png')");
		}
		
	},
	
}
