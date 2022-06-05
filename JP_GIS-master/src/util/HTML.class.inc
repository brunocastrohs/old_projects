<?php 


////CONTINUAR ATULIAZAÇÃO DE HTML E TESTAR EM INDEX

header("Content-Type: text/html; charset=UTF-8");
class HTML{
	
	const INSERTED = "<div class='successMessage'>Dados cadastrados.</div>";
    const ALREADY_INSERTED = "<div class=\"warningMessage\">Dados já cadastrados.</div>";
    const WARNING = "<div class=\"warningMessage\">Erro na operação.</div>";
    const NOT_INSERTED = "<div class=\"errorMessage\">Dados não foram cadastrados!</div>";
    const REMOVED = "<div class=\"successMessage\">Dados removidos.</div>";
    const NOT_REMOVED = "<div class=\"errorMessage\">Dados não foram removidos!</div>";
    const UPDATED = "<div class=\"successMessage\">Dados atualizados.</div>";
    const NOT_UPDATED = "<div class=\"errorMessage\">Dados não foram atualizados!</div>";
    const FILE_INSERTED = "<div class='successMessage'>O arquivo é valido e foi carregado com sucesso.</div>";
    const FILE_NOT_INSERTED = "<div class='errorMessage'>O arquivo nãe é valido e não foi carregado com sucesso.</div>";
    
    static $body = "";
    
    static $currentHomePageComboBox = "";
    
    static $comboSelected = false;
    
    static $colsNumber = 0;
    
    static $recordsNumber = 0; 
    
    static $genericVar = null;
    
    static $auxVar = null;
    
    static $isBodyless = false;
    
	static  function createDescription($content,$generics){
    	 
    	return "<p $generics>$content</p>";
    
    }
    
    static  function createTitle($content,$size){
    
    	return "<h$size>$content</h$size>";
    
    }
    
    static  function createList($content,$generics){
    	
    	return "<div class='infoList' $generics>$content</div>";
    
    }
    
    static  function createListItem($content,$generics){
    	 
    	return "<div class='infoListItem' $generics><span>$content</span></div>";
    
    }
    
    static  function createHeaderListItem($content,$generics){
    
    	return "<div class='infoListHeadItem' $generics><span style='margin-left:18px;'>$content</span></div>";
    
    }
    
	static  function createOption($value,$text,$generics){
			
		if($value == self::$comboSelected){
			return "<option selected $generics value='$value'>$text</option>";
		}
		
		return "<option $generics value='$value'>$text</option>";
		
	}
    
	static  function createEnhancedOption($attributes,$value,$text){
		
		if($text == "")
			$option = '<option value="">Selecione...</option>';
		else
        	$option = "<option $attributes value='$value'>$text</option>";
			
		return $option;
		
		
	}
    
	static function generateJsScript($code){
		
		echo "<script type='text/javascript'>$code</script>";
		
	}
	
	static function generateCSSScript($code){
	
		echo "<style>$code</style>";
	
	}
	
	static function reload(){
		
		echo "<script type='text/javascript'>window.location.reload()</script>";
		
	}
	
	static function location($url){
		
		echo "<script type='text/javascript'>window.location = ('$url')</script>";
		
	}
	
	static function createJsScript($code){
		
		return  "<script type='text/javascript'>$code</script>";
		
	}
	
	static function createColumn($value,$size,$isHead){
		
	    if($isHead && $value=='Geom')
			return "<TH style='width:$size%'>Geolocalizar</TH>";
		else if($isHead)
			    return "<TH style='width:$size%'>$value</TH>";
		else 
			return "<TD style='width:$size%'>$value</TD>";
		
	}
	
	static function createGeoColumn($value,$size){
		
		return "<TD style='width:$size%; cursor:pointer;'><input type='button' class='geomLocalizer' value='Localizar' onclick=\"VectorAction.setCenter('$value')\"/> </TD>";
		
	}
	
	static function createSPAN($id,$content,$generics){
	
		return "<span id='$id' $generics>$content</span>";
	
	}
	
	static function createSimpleDIV($id,$content,$generics){
	
		return "<div id='$id' $generics>$content</div>";
	
	}
	
	static function createHREF($text,$link,$generics){
	
		return "<a $generics href='$link'>$text</a>";
	
	}
	
	static function createFileHREF($text,$link,$generics){
	    
	    return "<a $generics href='$link' download='$text'>$text</a>";
	    
	}
	
	
	static function createIMG($src, $description, $generics){
	
		return "<img $generics alt='$description' src='$src' />";
	
	}
	
	/*********************************************************************************/
	/* HTML 5
	/*********************************************************************************/
	
	
	static function createWrapper($content){
	
		return "<div id='wrapper'>$content</div>";
	
	}
	
	static function createRobustWrapper($content,$wrapperStyleNumber){
	    
	    return "<div class='wrapper style$wrapperStyleNumber'>$content</div>";
	    
	}
	
	static function createDIV($content){
	
		return "<div class='mainDiv'>$content</div>";
	
	}
	
	static function createArticle($title,$contentBody){
	
		return "<article class='post'><section><h3>$title</h3>$contentBody</section></article>";
	
	}
	
	static function createRobustArticle($articleID,$size,$title,$contentBody,$contentFooter){
	    
	    return "<article id='$articleID' class='container{$size}'><header><h2>$title</h2></header>$contentBody<footer>$contentFooter</footer></article>";
	    
	}
	
	static function createHeader($title, $complement, $content){
	
		return "<header id='header' style='background-color:#00248A; color:white;'>
                <h1 class='logo'><a href='/JP_GIS/' style='color:white;'> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; $title <span id='complement'>$complement</span></a></h1>
                 $content </header>";
			
	}
	
	static function createNavigationMenu($items){
	
		return "<nav id='nav' class='links'><ul>$items</ul><div id='preloader'></div> <div id='messageBox'></div></nav>";
	
	}
	
	static function createNavigationItem($text,$link){
	
		return "<li><a href='$link'>$text</a></li>";
	
	}
	
	static function createNavigationItens($list){
	
		$str = "";
		
		foreach($list as $e){
			$str .= self::createNavigationItem($e['text'], $e['link']);
		}
		
		return $str;
	
	}
	
	static function createSideBarMenu($searchComponent, $listComponent, $loginComponent){
	
		return "<nav class='main'><ul><li class='menu'><a class='fa-bars' style='color:white;backgound:white;' href='#menu' onclick=".'$("#menu").show();'.">Menu</a></li></ul> <section id='menu'> $searchComponent $listComponent $loginComponent </section> </nav>";
	
	}
	
	static function createSideBarMenuList($title,$itens){
		//here - configurar o construtor de containers
		return "<section> $title <ul class='containers'> $itens </ul> </section>";
	
	}
	
	static function createSideBarSeachComponent($id, $action){
	
		return "<section><form class='search'><input type='text' name='$id' id='$id' $action  placeholder='Pesquisar' /></form></section>";
	
	}
	
	static function createSideBarFormComponent($itens){
	
		return "<section><form id='adminForm' name='adminForm'><ul class='actions vertical'>$itens</ul></form></section>";
	
	}
	
	static function createSideBarFormComponentItem($type, $text, $inputID, $action ){
	 
		if($type == 'password')
			return "<li><div class='12u 12u$(xsmall)'><input placeholder='$text' type='$type' name='$inputID' id='$inputID' value ='' /></div></li>";
		else if($type == 'text')
			return "<li><div class='12u 12u$(xsmall)'><input placeholder='$text' type='$type' name='$inputID' id='$inputID' value ='' /></div></li>";
		else if($type == 'button')
			return "<li><input type='button' onclick=\"$action\" value='$text'/></li>";
		
		return "<li></li>";
	
	}
	
	static function createRow($content){
	
		return "<div class='row'>$content</div>";
	
	}
	
	static function createImageFit($url){
	
		return "<span class='image fit'><img src='$url' alt='' /></span>";
	
	}
	
	static function createInfoDIV($content,$numberSize,$generics){
	
		return "<div $generics class='main'>$content</div>";
	
	}
	
	static function createButtonFit($text,$link, $size){
						
		return " <a href='$link' class='button $size fit'>$text</a> ";
	
	}
	
	static function createP($text){
		//<a href="#portfolio" class="button big scrolly">Conheça Nossas Ofertas</a>
			
		return "<p>$text</p>";
	
	}
	
	/*********************************************************************************/
	/* Form Page                                                                         */
	/*********************************************************************************/
	
	static function createForm($formID,$content,$generics){
	
		return "<form name='$formID' id='$formID' $generics><div class='row uniform'>$content</div></form>";
	
	}
	
	static function createFullFormInput($inputType,$inputID,$inputName,$inputLabel,$inputValue,$inputNumberSize,$generics){
	    
	    return "<div class='row'><div class='12u'><input type='$inputType' name='$inputName' id='$inputID' value ='$inputValue' placeHolder='$inputLabel' $generics /></div></div>";
	    
	}

	static function createFormInput($inputType,$inputID, $inputLabel,$inputValue,$inputNumberSize,$generics){
	
		if($inputType=="hidden"){
			return "<div class='{$inputNumberSize}u 12u$(xsmall)'><input type='$inputType' name='$inputID' id='$inputID' value ='$inputValue' $generics /></div>";
		}else if($inputType=="checkbox"){
			return "<div class='{$inputNumberSize}u 12u$(xsmall)'><input type='$inputType' name='$inputID' id='$inputID' value ='$inputValue' $generics /><label for='$inputID'>$inputLabel</label> </div>";
		}
		else
			return "<div class='{$inputNumberSize}u 12u$(xsmall)'><input placeholder='$inputLabel' type='$inputType' name='$inputID' id='$inputID' value ='$inputValue' $generics /></div>";
	
	}
	
	static function createLegendCheckbox($inputID, $inputLabel,$inputValue,$inputNumberSize,$generics){
	    
	    
	    return "<div ><input type='checkbox' name='$inputID' id='$inputID' value ='$inputValue' $generics /><label for='$inputID'>$inputLabel</label> </div>";
	    
	        
	}
	
	static function createTextArea($inputID,$inputLabel,$inputValue,$generics){
	
			return "<div class='12u$'><textarea placeholder='$inputLabel' name='$inputID' id='$inputID' $generics> $inputValue</textarea></div>";
	
	}
	
	static function createComboBox($inputID,$inputLabel,$data,$generics){
	
		$options = self::createOptions($data);
		
		return "<div class='12u$' id='row.$inputID'><div class='select-wrapper'><select name='$inputID' id='$inputID' $generics><option value=''>Selecione...</option>$options</select></div></div>";
	
	}
	
	static function createForeignAtrributeComboBox($inputID,$inputLabel,$data){
	
		$options = self::createOptions($data);
	
		return "<select name='searchKey' id='searchKey'><option value=''>Selecione...</option>$options</select>";
	
	}
	
	static function createRadioButtons($inputID,$inputLabel,$value,$generics){

		return
		$value=='t'?
		"<div class='4u 12u$(small)'>
		<input type='radio' name='$inputID' id='{$inputID}.1' value='true' checked>
		<label for='{$inputID}.1'>Sim</label>
		<input type='radio' name='$inputID' id='{$inputID}.2' value='false'>
		<label for='{$inputID}.2'>Não</label>
		</div>
		":
		"<div class='4u 12u$(small)'>
		<input type='radio' name='$inputID' id='{$inputID}.1' value='true'>
		<label for='{$inputID}.1'>Sim</label>
		<input type='radio' name='$inputID' id='{$inputID}.2' value='false' checked>
		<label for='{$inputID}.2'>Não</label>
		</div>";
			
	}
	
	static function createOptions($data){
	
		$options = "";
		
		if($data)
		foreach($data as $object){
			$options .= self::createOption($object['value'], $object['label'],'') ;
		}
		
		return "$options";
	
	}
	
	static function createFormButtonBar($content){
	
		return "<div class='row'><div class='12u$'><ul class='actions' style='float:right'>$content</ul></div></div>";
	
	}
	
	static function createFormButton($text,$action){
	
		return "<li><input type='button' onclick=\"$action\" value='$text'/></li>";
	
	}
	
	static function createCopyright(){
	
		return "<ul id='copyright'><li>&copy; Agência Pernambucana de Água e Clima. Todos os direitos reservados.</li></ul>";
	
	}
	
	/*********************************************************************************/
	/* Master Page                                                                         */
	/*********************************************************************************/
	
	static function createHomePageForm($schema,$table,$pk){
	
		$html = "<form method='post' id='homePageForm'>";
		
		//$html .= "<div class='12u' style='text-align:center;'><h2 style='color:white'>".(isset(DataBase::$tableLabels["$schema.$table"])?DataBase::$tableLabels["$schema.$table"]:str_replace("_", " ", strtoupper($table)) )."</h2></div>";
		
		if(SecurityUtil::checkPermission("INSERIR_".strtoupper("{$schema}_{$table}")))
			$html .= isset($_GET['isInfo'])?'':"<div class='row'><div class='5u'></div><div class='2u'><input type='button' name='name' id='name' value='Adicionar' onclick='Listener.sendFormRequest(false)' /></div><div class='5u'></div></div>";
		
		$html .= "<div class='row'><div class='2u'>".self::$currentHomePageComboBox."</div>";
		
		$html .= "<div class='6u' id='searchBar'><input type='text' name='searchKey' id='searchKey' placeholder='Valor de Pesquisa...' /></div>";
		
		$html .= "<div class='2u'><input type='button' name='searcher' id='searcher' value='Pesquisar' onclick='Listener.sendQuery();'/></div></div><br/></form>";
		
		return $html;
	
	}
	
	static function createAtrributeComboBox($inputID,$data){
	
		$options = self::createAtrributeOptions($data);
	
		self::$currentHomePageComboBox = "<div class='select-wrapper'><select onchange='Listener.changeSearchKey(this.value);' name='$inputID' id='$inputID'><option value=''>Selecione...</option>$options</select></div>";
	
	}
	
	static function createAtrributeOptions($data){
	
		$options = "";
	
		if($data)
		foreach($data as $object){
			if(!($object['type'] == 'action'))
				$options .= self::createOption("{$object['column']}.{$object['type']}", $object['label'],'') ;
		}
	
		return "$options";
	
	}
	
	/*********************************************************************************/
	/* Table Master Page                                                                         */
	/*********************************************************************************/
	
	static function createHomePageTable($headerData, $bodyData){
		
		$html = "<div class='table-wrapper'><table id='tableCRUD' >";
		
		$html .= self::createHomePageTableHeaders($headerData);
		
		$html .= self::createHomePageTableBody($bodyData);
		
		//$html .= "<tfoot><tr><td colspan='".self::$colsNumber."'></td><td id='rowCounter'>".self::$recordsNumber." Registros</td></tr></tfoot></table></div>";
		
		$html .= "<tfoot></tfoot></table></div>";
		
		self::generateJsScript("Util['rowCount'] = ".self::$recordsNumber.";");
		
		return $html;
	}
	
	static function createHomePageTableHeaders($headerData){
	
		$headers = "<thead><tr>";
	
		foreach($headerData as $header){
			
			if(strstr($header['type'], 'geom'))
				continue;
			else if((isset($_GET['isInfo']) || isset($_POST['isInfo'])) && $header['type'] == 'action'){
				continue;
			}
			
			$headers .="<th>{$header['label']}</th>";
		//	self::$colsNumber++;
		}
		
		$headers .= "<th></th></tr></thead>";
		
		return $headers;
	}
	
	static function createHomePageTableBody($bodyData){
	
		$body = !self::$isBodyless?"<tbody id='tableCRUDBody'>":"";
		
		$isFirst = true;
		
		$isFirstForColSpan = true;
		
		$currentId = 0;
		
		if(isset($bodyData[0]))
		foreach($bodyData as $row){
		
			$body .="<tr id='crudRow".self::$recordsNumber."' onclick='Listener.getRowIndex(".self::$recordsNumber.");'>";
			
			foreach($row as $td){
				
				if(strstr($td['type'], 'geom'))
					continue;
				else if((isset($_GET['isInfo']) || isset($_POST['isInfo'])) && $td['type'] == 'action'){
					continue;
				}
				
				if($isFirst)
					self::$auxVar = $currentId = $td['value'];
				if($isFirstForColSpan)
					self::$colsNumber++;
				
				$body .="<td>".self::checkTDValue($td)."</td>";
				
				$isFirst = false;
			}
			$body .= self::createHomePageTableBodyButtons($currentId) . "</tr>";
			$isFirst = true;
			$isFirstForColSpan = false;
			self::$recordsNumber++;
		}
	
		$body .= "<tr><td colspan='".self::$colsNumber."'></td><td id='rowCounter'>".self::$recordsNumber." Registros</td></tr>";
		
		$body .= !self::$isBodyless?"</tbody>":"";
		
		return $body;
	}
	
	static function checkTDValue($object){
	
		$value = $object['value'];
		
		if($object['type'] == 'date'){
			$value =  $object['value'];
		}
		else if($object['type'] == 'boolean'){
			$value = $object['value']=='t'?'Sim':'Não';
		}
		else if($object['type'] == 'fk'){
			$value =  DAO::$fksValues[$object['column']][$object['value']];
		}
		else if($object['type'] == 'action'){
			$url = "/JP_GIS/index.php?x=foreignHomePageCRUD&y={$object['actionTable']}&z={$object['nativeTable']}&k=$value";
			
			$value =  "<span style='cursor:pointer;color:blue;text-decoration: underline;' onclick='window.location = \"$url\"'>Cadastrar {$object['label']}</span>";
		}
		else if(isset(DataBase::$passwordColumns[Controller::$schema.".".Controller::$table.".".$object['column']])){
			$value = '----------';
		}
		else if(isset(DataBase::$tableWithFiles[self::$genericVar]) && DataBase::$tableWithFiles[self::$genericVar] == $object['column']){
			
			$table = explode(".", self::$genericVar);
			
			$file = "{$table[1]}_".self::$auxVar.".".(self::getFileExtension($object['value']));
			
			$value =  "<span style='cursor:pointer;color:blue;text-decoration: underline;' onclick='Listener.getFile(\"/JP_GIS/tmp/$file\")'>{$object['value']}</span>";
		}
		
		return $value;
	}
	
	static function getFileExtension($fileName){
	    
	    $fileExtension = explode(".", $fileName);
	    
	    return $fileExtension[(count($fileExtension)-1)];
	    
	    
	}
	
	static function createHomePageTableBodyButtons($id){
		
		if(isset($_GET['isInfo']) || isset($_POST['isInfo'])){
			
			$isInfo = isset($_GET['isInfo'])?$_GET['isInfo']:$_POST['isInfo'];
			
			return "<td><div class='infoButton' onclick='window.location = \"/JP_GIS/index.php?x=info&y=$isInfo&z=$id\"'></div></td>";
			
		}
		else{
			
			$updateB = SecurityUtil::checkPermission("ATUALIZAR_".strtoupper(Controller::$schema."_".Controller::$table))?"<div class='updateButton' onclick='Listener.sendFormRequest($id)'></div>":"";
			
			$removeB = SecurityUtil::checkPermission("REMOVER_".strtoupper(Controller::$schema."_".Controller::$table))?"<div class='removeButton' onclick='Listener.sendRequest(\"remove\",$id,this)'></div>":"";
			
			return "<td>$updateB $removeB</td>";
			
		}
			
	}
	
	/*********************************************************************************/
	/* GIS & OTHER RESOURCES*/
	/*********************************************************************************/
	
	static function generateUtilGISVariables($w){
	
		echo "<script type='text/javascript'>Util['idGIS']='$w';</script>";
	
	}
	
	static function generateUtilVariables($x,$y,$z,$k,$w){
	
		echo isset($_GET['isInfo'])?"<script type='text/javascript'>Util['x']='$x';Util['y']='$y';Util['z']='$z';Util['k']='$k';Util['w']='$w';Util['isInfo']=true;</script>":"<script type='text/javascript'>Util['x']='$x';Util['y']='$y';Util['z']='$z';Util['k']='$k';Util['w']='$w';</script>";
	
	}
	
	static function generateUtilForeignVariables($foreignColumn,$foreignValue){
	
		echo "<script type='text/javascript'>Util['fC']='$foreignColumn';Util['fV']='$foreignValue';</script>";
	
	}
	
	static function createFileFormVariables($x,$y,$z,$k,$w){
	
		return "<div class='row'><input type='hidden' name='action' value='fileUpload'/><input type='hidden' name='y' value='$y'/><input type='hidden' name='z' value='$z'/><input type='hidden' name='k' value='$k'/><input type='hidden' name='w' value='$w'/></div>";
	
	}
	
}

?>