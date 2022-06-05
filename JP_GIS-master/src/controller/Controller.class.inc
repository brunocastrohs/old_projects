<?php
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/config/System.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/dao/DAO.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/util/HTML.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/util/SecurityUtil.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/controller/VectorController.class.inc");

class Controller{
	
	static $pk = null;
	
	static $currentBody = "";
	
	static $genericVar = null;
	
	static $schema = null;
	
	static $table = null;
	
	static $isForeign = false;
	
	static function save($schema,$table,$pk,$object){
	
		$hasNotNull = self::checkForNotNull($schema, $table, $object);
	
		if($hasNotNull) {
			echo "Never happens....";
		}
	
		else{
	
			$rst = DAO::save($schema,$table, $pk, $object);
	
			if ($rst) {
				echo HTML::INSERTED;
			} else {
				echo HTML::NOT_INSERTED;
			}
			
		}
	
	}
	
	static function checkForNotNull($schema,$table,$object){
	
		$columns = explode(',', implode(",",array_keys(DataBase::$notNullColumns)));
	
		$hasNotNull = false;
	
		$notNullColumns = "";
	
		foreach($columns as $str){
				
			$column = explode(".", $str);
				
			if($column[0] == $schema &&  $column[1] == $table){
					
				$column = $column[2];
				
				if(!isset($object[$column]) || $object[$column] == null || $object[$column] == "" || $object[$column] == " " || strstr($object[$column], "null") ){
					$hasNotNull = true;
					$label = isset(DataBase::$labels[$schema.".".$table])&&isset(DataBase::$labels[$schema.".".$table][$column])?DataBase::$labels[$schema.".".$table][$column]:ucwords(str_replace("_"," ",$column));
					$notNullColumns .= "</br>- ". $label;
				}
			}
				
		}
	
		if($hasNotNull)
			return "<br/>Verifique os campos obrigatórios:$notNullColumns";
			else
				return false;
	
	}
	
	static function update($schema,$table,$object,$pk,$id){
	
		$rs = DAO::update($schema,$table,$object,$pk,$id);
	
		if ($rs) {
			echo HTML::UPDATED;
		} else {
			echo HTML::NOT_UPDATED;
		}
	
	}
	
	static function remove($schema,$table,$pk,$id){
	
	$rst = DAO::remove($schema,$table,$pk,$id);
	
	if ($rst) {
		echo HTML::REMOVED;
    } else {
		echo HTML::NOT_REMOVED;
	}
	
	
	}
	
	static function generateNavBar(){
	
		//here
		$searchComponent = HTML::createSideBarSeachComponent('pesq', 'onclick=""');
		
		//here
		$loginItens = HTML::createSideBarFormComponentItem("Login", "", 'big',"");
		$loginComponent = HTML::createSideBarFormComponent($loginItens);
		
		$itensListComponent  = HTML::createNavigationItem("Mapa 1", "#");
		$itensListComponent .= HTML::createNavigationItem("Mapa 2", "#");
		$itensListComponent .= HTML::createNavigationItem("Mapa 3", "#");
		
		$listComponent = HTML::createSideBarMenuList($itensListComponent);
		$siderBar = HTML::createSideBarMenu($searchComponent, $listComponent, $loginComponent);
		
		$navBar = null;
		
		$navList  = HTML::createNavigationItem("Mapa", "#");
		$navList .= HTML::createNavigationItem("Pesquisa", "#");
		$navList .= HTML::createNavigationItem("Cadastro", "#");
		$navBar = HTML::createNavigationMenu($navList);
		
		$headerContent = $navBar?$navBar." ".$siderBar:$siderBar;
		
		$header = HTML::createHeader('APAC', $headerContent);
		
		echo $header;
	}
	
	static function generateHomePageFromTable($schema,$table){
	
		//echo SecurityUtil::checkPermission("VISUALIZAR_".strtoupper("{$schema}_{$table}"))?'':HTML::generateJsScript("history.back();");
		
		self::generateNavBar();
		
		self::$table = $table;
		
		self::$schema = $schema;
	
		$pk = DAO::retrievePrimaryKey($schema, $table);
		
		$columns = DAO::retrieveColumns($schema, $table, "");
		
		HTML::$genericVar = "$schema.$table";
		
		$records = DAO::retrieveRecords($columns, "");
		
		HTML::createAtrributeComboBox("attributeCombo",$columns);
		
		HTML::generateUtilVariables("", $schema, $table, $pk, "");
		
		self::$currentBody = HTML::createHomePageForm($schema, $table, $pk);	
	
		self::$currentBody .= HTML::createHomePageTable($columns, $records);
		
		$article = HTML::createArticle((isset(DataBase::$tableLabels["$schema.$table"])?DataBase::$tableLabels["$schema.$table"]:str_replace("_", " ", strtoupper($table)) ), self::$currentBody );
		
		$div = HTML::createDIV($article);
		
		echo HTML::createWrapper($div);
	
	}
	
	static function retrieveHomePageQueryData($schema,$table,$pk,$attribute,$value){
	
		DAO::$pk = self::$pk = $pk;
		
		self::$table = $table;
		
		self::$schema = $schema;
		
		HTML::$genericVar = "$schema.$table";
	
		$columns = DAO::retrieveColumns($schema, $table, "");
	
		$params = self::checkQueryParam($attribute, $value);
		
		$records = DAO::retrieveRecords($columns, $params);
	
		HTML::$isBodyless = true;
		
		if(isset($records[0])){
		
			self::$currentBody .= HTML::createHomePageTableBody($records);
	
			echo self::$currentBody;
		
		}else{
	
			echo "Sem registros...";
		
		}
	}
	
	static function generateForeignHomePageFromTable($schema,$table,$foreignSchema,$foreignTable,$id){
	
		echo SecurityUtil::checkPermission("VISUALIZAR_".strtoupper("{$schema}_{$table}"))?'':HTML::generateJsScript("history.back();");
		
		self::generateNavBar();
		
		self::$table = $table;
		
		self::$schema = $schema;
	
		$foreignColumn = DAO::retrieveForeignKey($schema, $table, $foreignSchema, $foreignTable);
	
		$pk = DAO::retrievePrimaryKey($schema, $table);
	
		$columns = DAO::retrieveColumns($schema, $table, "");
	
		HTML::$genericVar = "$schema.$table";
	
		$records = DAO::retrieveRecords($columns, " $foreignColumn = $id ");
	
		HTML::createAtrributeComboBox("attributeCombo",$columns);
	
		HTML::generateUtilVariables("", $schema, $table, $pk, "");
		
		HTML::generateUtilForeignVariables($foreignColumn, $id);
	
		self::$currentBody = HTML::createHomePageForm($schema, $table, $pk);
	
		self::$currentBody .= HTML::createHomePageTable($columns, $records);
	
		$article = HTML::createArticle((isset(DataBase::$tableLabels["$schema.$table"])?DataBase::$tableLabels["$schema.$table"]:str_replace("_", " ", strtoupper($table)) ), self::$currentBody );
		
		$div = HTML::createDIV($article);
		
		echo HTML::createWrapper($div);
			
	}
	
	static function retrieveForeignHomePageQueryData($schema,$table,$pk,$attribute,$value,$foreignAtribute,$foreignValue){
	
		DAO::$pk = self::$pk = $pk;
		
		self::$table = $table;
		
		self::$schema = $schema;
	
		HTML::$genericVar = "$schema.$table";
	
		$columns = DAO::retrieveColumns($schema, $table, "");
	
		$params = " $foreignAtribute = $foreignValue AND ";
		
		$params .= self::checkQueryParam($attribute, $value);
	
		$records = DAO::retrieveRecords($columns, $params);
	
		HTML::$isBodyless = true;
	
		if(isset($records[0])){
	
			self::$currentBody .= HTML::createHomePageTableBody($records);
	
			echo self::$currentBody;
	
		}else{
	
			echo "Sem registros...";
	
		}
	}
	
	static function checkQueryParam($params,$value){
		
		$params = explode(".", $params);
	
		$clause= "";
		
		if($value == ''){
			return $params[0].' is not null';
		}
		
		if(strstr($params[1], 'double precision') || $params[1] == 'integer' || strstr($params[1], 'numeric')){
			$clause = "{$params[0]} = $value";			
		}
		else if($params[1] == 'date' || strstr($params[1],"time")){
			$clause = "{$params[0]} $value ";
		}
		else if($params[1] == 'boolean'){
			$clause = "{$params[0]} = $value";
		}
		else if($params[1] == 'fk'){
			$clause = "{$params[0]} = $value";
			
		}
		else{
			$clause = "{$params[0]} ilike '%$value%'";
		}	
				
		return $clause;
	
	}
	
	static function generateGeoFormFromTable($schema,$table, $lat, $long){
	
		$generics = "";
		
		self::$schema = $schema;
		
		self::$table = $table;
	
		DAO::$pk = self::$pk = $pk;
	
		$buttonToolbar = '';//self::generateCRUDFormBar($schema,$table,$pk,$id);
	
		$inputs = self::generateFormInputs($schema, $table, $pk, $id);
	
		$formContent = ($inputs.$buttonToolbar);
			
		$form = HTML::createForm("crudFORM", $formContent,$generics);
	
		$footer = '<div id="gisForm"></div>';//HTML::createCopyright();
	
		$article = HTML::createArticle((isset(DataBase::$tableLabels["$schema.$table"])?DataBase::$tableLabels["$schema.$table"]:str_replace("_", " ", strtoupper($table)) ), $form );
		
		$div = HTML::createDIV($article);
		
		echo HTML::createWrapper($div);
	
	}
	
	static function generateFormFromTable($schema,$table,$pk,$id){
		
		self::generateNavBar();
		
		$generics = "";
		
		self::$schema = $schema;
		
		self::$table = $table;
		
		HTML::generateUtilVariables("", $schema, $table, $pk, "");
		
		DAO::$pk = self::$pk = $pk;
		
		$buttonToolbar = self::generateCRUDFormBar($schema,$table,$pk,$id);
	
		$inputs = self::generateFormInputs($schema, $table, $pk, $id);
	
		$formContent = ($inputs.$buttonToolbar);
	
		if(isset(DataBase::$tableWithFiles["{$schema}.{$table}"]))
			$generics = "enctype='multipart/form-data'";
			
		$form = HTML::createForm("crudFORM", $formContent,$generics);
	
		$footer = HTML::createCopyright();
	
		$article = HTML::createArticle((isset(DataBase::$tableLabels["$schema.$table"])?DataBase::$tableLabels["$schema.$table"]:str_replace("_", " ", strtoupper($table)) ), $form );
		
		$div = HTML::createDIV($article);
		
		echo HTML::createWrapper($div);
		
	}
	
	static function generateFormFileFromTable($schema,$table,$pk,$id){
	
		self::generateNavBar();
	
		self::$schema = $schema;
		
		self::$table = $table;
		
		HTML::generateUtilVariables("", $schema, $table, $pk, "");
		
		DAO::$pk = self::$pk = $pk;
	
		$buttonToolbar = self::generateCRUDFormFileBar($schema,$table,$pk,$id);
	
		$inputs = self::generateFormInputs($schema, $table, $pk, $id);
	
		$inputs .= HTML::createFileFormVariables("", $schema, $table, $pk, $id) ;
		
		$formContent = ($inputs.$buttonToolbar);
			
		$form = HTML::createForm("crudFORM", $formContent,"enctype='multipart/form-data'");
	
		$footer = HTML::createCopyright();
	
		$article = HTML::createArticle((isset(DataBase::$tableLabels["$schema.$table"])?DataBase::$tableLabels["$schema.$table"]:str_replace("_", " ", strtoupper($table)) ), $form );
		
		$div = HTML::createDIV($article);
		
		echo HTML::createWrapper($div);
	
	}

	static function generateFormInputs($schema,$table,$pk,$id){
	
		
		$inputs = "";
	
		$columns = DAO::retrieveColumns($schema, $table, "");
		
		if($id){
			
			$object = DAO::retrieveRecords($columns, " $pk = $id ");
			
			if($object)
			foreach($object[0] as $column){					
				$inputs .= self::generateFormUpdateInput($table,$column);
			}
			
		}
		else{
			
			foreach($columns as $column){				
				$inputs .= self::generateFormInsertInput($table,$column);			
			}
		}
		
		return $inputs;
	
	}
	
	static function generateFormInsertInput($table,$object){
	
		$value = "''";
		
		$type = '';
		
		$label = $object['label'];

		$generics = "";
		
		if(isset($object['isHidden']) || self::$pk == $object['column']){
			$value = "";
			$type = 'hidden';
		}
		else if(strstr($object['type'], 'timestamp')){
			$value =  date("d/m/Y H:i:s");
			$type = "hidden";
		}
		else if(strstr($object['type'], 'double precision') || $object['type'] == 'integer' || strstr($object['type'], 'numeric')){
			$value =  '';
			$type = "number";
		}
		else if($object['type'] == 'date'){
			$value =  '';
			$type = "date";
		}
		else if($object['type'] == 'boolean'){
			$value = 'false';
			return HTML::createRadioButtons($object['column'], $label, $value, $generics);
		}
		else if($object['type'] == 'fk'){
			return HTML::createComboBox($object['column'], $label, $object['fk_data']['data'], "12", '');
		}
		else if(strstr($object['type'], 'character')){
			return HTML::createTextArea($type, $object['column'], $object['column'], $label, 'Descrição', "12", $generics) ;
		}
		else if(isset(DataBase::$tableWithFiles[self::$schema.".".self::$table]) && DataBase::$tableWithFiles[self::$schema.".".self::$table] == $object['column']){
			$value = '';
			$type = "file";
		}
		else if(isset(DataBase::$passwordColumns[self::$schema.".".self::$table.".".$object['column']])){
			$value = '';
			$type = "password";
		}
		else if(strstr($object['type'], 'geom') || ($object['type'] == 'action')){
			return '';
		}
		else{
			$value = '';
			$type = "text";
		}	

		//KL Manager Only
		if(isset(DataBase::$mergeFormColumnEspecialValue[self::$schema.".".self::$table.".".$object['column']]) && self::$table=='nivel' && $object['column'] == 'ordem' && self::$isForeign){
			$value = 1+DAO::retrieveTableOnlyValueByQuery("SELECT ordem FROM ".self::$schema.".".self::$table." WHERE ".self::$isForeign['fC']." = ".self::$isForeign['fV']." ORDER BY ordem DESC LIMIT 1");
		}else if(isset(DataBase::$mergeFormColumnEspecialValue[self::$schema.".".self::$table.".".$object['column']]) && self::$table=='nivel' && $object['column'] == 'descricao' && self::$isForeign){
			$value = "Nível ".(1+DAO::retrieveTableOnlyValueByQuery("SELECT ordem FROM ".self::$schema.".".self::$table." WHERE ".self::$isForeign['fC']." = ".self::$isForeign['fV']." ORDER BY ordem DESC LIMIT 1"));
		}
			
		else if(isset(DataBase::$mergeFormColumnEspecialValue[self::$schema.".".self::$table.".".$object['column']]) && self::$table=='subnivel' && $object['column'] == 'ordem' && self::$isForeign){
			$value = 1+DAO::retrieveTableOnlyValueByQuery("SELECT ordem FROM ".self::$schema.".".self::$table." WHERE ".self::$isForeign['fC']." = ".self::$isForeign['fV']." ORDER BY ordem DESC LIMIT 1");
		}else if(isset(DataBase::$mergeFormColumnEspecialValue[self::$schema.".".self::$table.".".$object['column']]) && self::$table=='subnivel' && $object['column'] == 'descricao' && self::$isForeign){
			$value = "Subnível ".(1+DAO::retrieveTableOnlyValueByQuery("SELECT ordem FROM ".self::$schema.".".self::$table." WHERE ".self::$isForeign['fC']." = ".self::$isForeign['fV']." ORDER BY ordem DESC LIMIT 1"));
		}
		
		return HTML::createFormInput($type, $object['column'], $label, $value, "12", $generics);
	
	}

	static function generateFormUpdateInput($table,$object){
	
		$value = $object["value"];
		
		$type = '';
		
		$label = $object['label'];

		$generics = "";
		
		if(isset($object['isHidden']) || self::$pk == $object['column'])
			$type = 'hidden';
		else if(strstr($object['type'], 'timestamp')){
			$value =  date("d/m/Y");
			$type = "hidden";
		}
		else if(strstr($object['type'], 'double precision') || $object['type'] == 'integer' || strstr($object['type'], 'numeric')){
		//	$value =  '';
			$type = "number";
		}
		else if($object['type'] == 'date'){
			//$value =  '';
			$type = "date";
		}
		else if($object['type'] == 'boolean'){
			//$value='';
			return HTML::createRadioButtons($object['column'], $label, $value, $generics);
		}
		else if($object['type'] == 'fk'){
			
			HTML::$comboSelected = $object['value'];
			
			if(self::$table == 'vector' && $object['column']=='tipo'){
				$generics = " onchange='VectorAction.showVectorTypeOption(this.value)'";
				HTML::generateJsScript("VectorAction.showVectorTypeOption('{$object['value']}')");
			}
			
			return HTML::createComboBox($object['column'], $label, $object['fk_data']['data'], "12", $generics);
			
		}
		else if(isset(DataBase::$tableWithFiles[self::$schema.".".self::$table]) && DataBase::$tableWithFiles[self::$schema.".".self::$table] == $object['column']){
			$value = '';
			$type = "file";
		}
		else if(isset(DataBase::$passwordColumns[self::$schema.".".self::$table.".".$object['column']])){
			$value = base64_decode($value);
			$type = "password";
		}
		else if(strstr($object['type'], 'character')){
			return HTML::createTextArea($type, $object['column'], $object['column'], $label, $value, "12", $generics) ;
		}
		else if(strstr($object['type'], 'geom')){
			return '';
		}else if($object['type'] == 'action'){
			return '';
		}else{
			//$value = '';
			$type = "text";
		}	

		return HTML::createFormInput($type, $object['column'],  $label, $value, "12", $generics);
		
	}
	
	static function generateForeignQueryAttributes($schema,$table,$field){
	
		$list = DAO::retrieveForeignKeys($schema, $table, " AND kcu.column_name = '$field'");
		
		echo HTML::createForeignAtrributeComboBox($field, $field, $list[$field]['data']);
	
	}
	
	static function generateCRUDFormBar($schema,$table,$pk,$id){
		$buttons = "";
		if($id && SecurityUtil::checkPermission("ATUALIZAR_".strtoupper("{$schema}_$table"))){
			$buttons = HTML::createFormButton("Atualizar", "Listener.sendRequest('update',$id,this)");
		}else if(SecurityUtil::checkPermission("INSERIR_".strtoupper("{$schema}_$table")))
			$buttons = HTML::createFormButton("Salvar", "Listener.sendRequest('merge','',this)");
	
		
		return HTML::createFormButtonBar($buttons);
	
	}
	
	static function generateCRUDGeomFormBar($schema,$table,$pk,$id){
		$buttons = "";
		
		if($id && SecurityUtil::checkPermission("ATUALIZAR_".strtoupper("{$schema}_$table"))){
			$buttons = HTML::createFormButton("Atualizar", "Listener.sendGeoFormRequest('update',$id,this)");
		}else if(SecurityUtil::checkPermission("INSERIR_".strtoupper("{$schema}_$table")))
			$buttons = HTML::createFormButton("Salvar", "Listener.sendGeoFormRequest('merge','',this)");
	
	
		return HTML::createFormButtonBar($buttons);
	
	}
	
	static function generateCRUDFormFileBar($schema,$table,$pk,$id){
		$buttons = "";
		
		if($id && SecurityUtil::checkPermission("ATUALIZAR_".strtoupper("{$schema}_$table"))){
			$buttons = HTML::createFormButton("Atualizar", "Listener.sendFileRequest(this)");
		}else if(SecurityUtil::checkPermission("INSERIR_".strtoupper("{$schema}_$table")))
			$buttons = HTML::createFormButton("Salvar", "Listener.sendFileRequest(this)");
	
	
		return HTML::createFormButtonBar($buttons);
	
	}
	
	static function generateCRUDMultiFormBar($list,$id){
	
		$data = "[";
		
		$isFirst = true;
		
		foreach($list as $object){
			$data .= $isFirst?"'{$object[0]}:{$object[1]}:{$object[2]}'":",'{$object[0]}:{$object[1]}:{$object[2]}'";
			$isFirst = false;
		}
		
		$data .= "]";
		
		HTML::generateJsScript("Listener.multiForm = $data;");
		$buttons = "";
		
		if($id && SecurityUtil::checkPermission("ATUALIZAR_PESQUISA_IDENTIFICACAO")){
			$buttons = HTML::createFormButton("Atualizar", "Listener.sendMultiFormRequest('multiUpdate',$id,this)");
		}else if(SecurityUtil::checkPermission("INSERIR__PESQUISA_IDENTIFICACAO"))
			$buttons = HTML::createFormButton("Salvar", "Listener.sendMultiFormRequest('multiMerge','',this)");
	
	
		return HTML::createFormButtonBar($buttons);
	
	}
	
	static function retrieveGISRecords($schema,$table, $params){
	
		$params = str_replace("like", "ilike", $params);
	
		$rows = DAO::retrieveGISRecords($schema,$table,$params);
	
		$tableLabel = strtoupper(str_replace("_", " ", $table));
	
		if(isset($rows[0])){
				
			$tableSize = count($rows[0])+1;
				
			$size = 100/$tableSize;
	
			$flag = 0;
	
			$htmlTable ="<div id='size.$table' style='width:inherit;'>
            <h4>$tableLabel</h4>
            <TABLE class='gisRecords' cellspacing='0'><TR><THEAD>";
	
			foreach($rows[0] as $col){
			    if(!$col['isHidden']) 
				 $htmlTable .= HTML::createColumn($col['label'],$size,true);
	
			}
	
			$htmlTable .="</THEAD></TR><TBODY>";
		
			foreach($rows as $row){
					
				$htmlTable .= '<TR>';
					
				foreach ($row as $col){
				    if(!$col['isHidden'])
						
				        if($table == System::TR_TABLE && $col['column'] == System::TR_COLUMN){
				            $local = "<a target='_blank' href='".System::TR_URL."?".System::TR_COLUMN."=".$col['value']."'>".$col['value']."</a>";
				            $htmlTable .= HTML::createColumn($local,$size,false);
				        }
				        else if(!strstr($col['type'],'geometry')){
				            $htmlTable .= HTML::createColumn($col['value'],$size,false);
				        }
						else{
	
							$htmlTable .= HTML::createGeoColumn($col['value'],$size);
		
						}
				}
					
				$htmlTable .= '</TR>';
					
				$flag++;
			}
	
			$htmlTable .= "</TBODY></TABLE></div>";
	
			echo $htmlTable;
		} else
			echo "<div id='size.$table' style='display:none;'>3</div>Nenhum resultado encontrado para a sua consulta em $tableLabel.<div id='number.$table' style='visibility:hidden;'>, nenhum...</div><br/>";
	
	}
	
	static function retrieveAutoCompleteItens($schema,$table, $attrib, $key){
	    
	    $params = "$attrib ilike '%$key%'";
	    
	    $rows = DAO::retrieveAutoCompleteRecords($schema, $table, $attrib, $params);
	    
	    if(isset($rows[0])){
	        
	        $htmlTable = "<ul class='autoCompleteList'>";
	        
	        foreach($rows as $row){
	            
	            $htmlTable .= "<li onclick='VectorAction.selectAutoCompleteItem(this)'>$row</li>";
	            
	        }
	        
	        $htmlTable .= "</ul>";
	        
	        echo $htmlTable;
	        
	    } else
	        
	        echo "";
	        
	}
	
	
}

?>