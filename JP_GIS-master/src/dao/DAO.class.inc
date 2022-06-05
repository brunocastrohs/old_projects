<?php
require_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/context/DatabaseContext.class.inc");

class DAO{

	static $columns= null;
	
	static $schema= null;
	
	static $table= null;

	static $pk = null;
	
	static $fks = null;
	
	static $fksValues = array();
		
	//META
	
	static function save($schema,$table,$pk, $object){
	
		$object[$pk] = self::retrieveIdCount($schema, $table, $pk)+1;

		if($table == 'usuario')
			$object['usu_senha'] = "'".base64_encode(str_replace("'", "",($object['usu_senha']))) ."'";
		
		$sql = "INSERT INTO $schema.$table (". implode(",",array_keys($object)).") VALUES(".implode(",",array_values($object)).")";
	
		$rs = DatabaseContext::execute($sql);
	
		return $rs;
	
	}
	
	static function update($schema,$table,$object,$pk,$id){
	
		if($table == 'usuario')
			$object['usu_senha'] = "'".base64_encode(str_replace("'", "", ($object['usu_senha'])))."'";
		
		$sql = "UPDATE $schema.$table SET ". self::generateUpadteString($object) ." WHERE $pk = $id";
	
		$rs = DatabaseContext::execute($sql);
		
		return $rs;
	
	}
	
	static function remove($schema,$table,$pk,$id){
	
		$sql ="DELETE FROM $schema.$table WHERE $pk = $id";
	
		$rs = DatabaseContext::execute($sql);
	
		//if(isset($_SESSION['user_ufcmanager']['id']))
			//DatabaseContext::mergeUserAction($_SESSION['user_ufcmanager']['id'],$sql);
	
		return $rs;
	
	}
	
	static function retrieveIdCount($schema,$table, $pk){
	
		$sql = "select max($pk) from $schema.$table";
	
		$rs = DatabaseContext::execute($sql);
			
		$list = array();
			
		while($row = pg_fetch_row($rs)){
			if($row[0] > 0)
				return $row[0];
			else
				return 0;
		}
	
	}
	
	static function countTable($schema,$table, $pk){
	
		$sql = "select count($pk) from $schema.$table";
	
		$rs = DatabaseContext::execute($sql);
			
		$list = array();
			
		while($row = pg_fetch_row($rs)){
			if($row[0] > 0)
				return $row[0];
			else
				return 0;
		}
	
	}
	
	static function retrieveSchemas(){
	
		$sql = "SELECT DISTINCT pg_tables.schemaname as schema FROM pg_tables WHERE pg_tables.schemaname <> 'topology' AND pg_tables.schemaname <> ALL (ARRAY['pg_catalog'::name, 'information_schema'::name, 'pg_toast'::name]) ORDER BY pg_tables.schemaname";
	
		$rs = DatabaseContext::execute($sql);
	
		$list = array();
			
		while($row = pg_fetch_row($rs)){
	
			array_push($list, $row[0]);
	
		}
	
		return $list;
	
	}
	
	static function generateSchemasClause($schemas){
	
		$str = "";
	
		foreach($schemas as $schema){
	
			if($str == "")
				$str .= " AND pg_tables.schemaname = '{$schema}' ";
			else
				$str .= " OR pg_tables.schemaname = '{$schema}' ";
		}
	
		return $str;
	
	}
	
	static function retrieveTables($schemas){
	
		$sql = "SELECT pg_tables.schemaname, pg_tables.tablename FROM pg_tables WHERE pg_tables.schemaname <> ALL (ARRAY['pg_catalog'::name, 'information_schema'::name, 'pg_toast'::name]) ".self::generateSchemasClause($schemas)." ORDER BY pg_tables.schemaname, pg_tables.tablename";
	
		$rs = DatabaseContext::execute($sql);
	
		$flag = 0;
			
		$list = array();
			
		while($row = pg_fetch_row($rs)){
	
			$list[$flag]['schema'] 	= $row[0];
			$list[$flag]['table'] 	= $row[1];
				
			$flag ++;
			
		}
	
		return $list;
	
	}
	
	static function retrieveColumns($schema,$table,$clauses){

		self::$schema = $schema;
		
		self::$table = $table;
		
		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod),attnum FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT oid FROM pg_class WHERE oid = '$schema.$table'::regclass) and pg_attribute.attnum > 0 and pg_attribute.ATTISDROPPED = 'f' and format_type(pg_attribute.atttypid, pg_attribute.atttypmod) <> 'geometry' $clauses";
	
		$rs = DatabaseContext::execute($sql);
	
		$flag = 0;
		
		self::$fks = self::retrieveForeignKeys($schema, $table,'');
			
		$list = array();
	
		while($row = pg_fetch_row($rs)){
	
			$list[$flag]['column'] = $row[0] ;
			$list[$flag]['label'] = isset(DataBase::$labels["$schema.$table"])&&isset(DataBase::$labels["$schema.$table"][$row[0]])?DataBase::$labels["$schema.$table"][$row[0]]:ucwords(str_replace("_"," ",$row[0]));
			$list[$flag]['type'] = $row[1];
			$list[$flag]['id'] = $row[2];
			
			//foreign keys function
			if(self::$pk != $row[0] && isset(self::$fks["{$row[0]}"])){
				$list[$flag]['fk_data'] = self::$fks["{$row[0]}"];
				$list[$flag]['type'] = "fk";
			}
			
			if(isset(DataBase::$hiddenColumns[self::$schema.".".self::$table.".".$row[0]])){
				$list[$flag]['isHidden'] = true;
			}
			
			$flag ++;
		}
		
		if(isset(DataBase::$actionColumns["$schema.$table"]))
			$list =	self::checkForActionColumns($schema, $table, $list);
			
		return $list;
	
	}
	
	static function checkForActionColumns($schema,$table,$list){
		
		$actionColumns = explode(";", DataBase::$actionColumns["$schema.$table"]) ;
		
		foreach($actionColumns as $column){

			if($column){
				$aux = array(
					"type"=>'action',
					"column"=>"$column",
					"label"=> isset(DataBase::$labels["$schema.$table"])&&isset(DataBase::$labels["$schema.$table"][$column])?DataBase::$labels["$schema.$table"][$column]:ucwords(str_replace("_"," ",$column))
					);
			
				array_push($list, $aux); 
			}
		}
			
		return $list;
			
	}
	
	static function retrieveRecords($cols,$params){
	
		$schema = self::$schema;
		
		$table = self::$table;
		
		if($params != '')
			$params = " WHERE $params";
	
		$select = self::generateSelectColumns($cols);
	
		$sql = "SELECT $select FROM $schema.$table $params ORDER BY ".self::$pk." DESC";
			
		$rs = DatabaseContext::execute($sql);
	
		$flag = 0; 
			
		$records = array();
	
		$prefix = '';
		
		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){
	
				$records[$flag][$c]['column'] = $cols[$c]['column'] ;
				$records[$flag][$c]['type'] = $cols[$c]['type'];
				$records[$flag][$c]['label'] = $cols[$c]['label'];
				
				if(strstr($cols[$c]['type'], 'date') || strstr($cols[$c]['type'], 'timestamp')){
					$records[$flag][$c]['value'] = self::formatDataToUserFormat($row[$c],$cols[$c]['type']);
				}
				else if(self::$pk != $cols[$c]['column'] && isset(self::$fks["{$cols[$c]['column']}"])){
					$records[$flag][$c]['value'] =  preg_replace( "/\r|\n/", "", $row[$c] );
					$records[$flag][$c]['fk_data'] = self::$fks["{$cols[$c]['column']}"];
					$records[$flag][$c]['type'] = "fk";
				}
				else if(($cols[$c]['type'] == 'action')){
					$records[$flag][$c]['nativeTable'] = "$schema.$table";						
					$records[$flag][$c]['actionTable'] = $cols[$c]['column'];		
					$records[$flag][$c]['value'] = $records[$flag][0]['value'];
				}
				else
					$records[$flag][$c]['value'] =  preg_replace( "/\r|\n/", "", $row[$c] );
					
				if(isset($cols[$c]['isHidden'])){
					$records[$flag][$c]['isHidden'] = $cols[$c]['column'];
				}
				
			}
	
			$flag ++;
		}
	
		if($flag > 0)
			return $records;
		else
			return false;
	
	}
	
	static function retrieveColumnValues($schema, $table, $column){
	
			$sql = "select distinct($column) from $schema.$table order by $column";
				
			$rs = DatabaseContext::execute($sql);
	
			$flag = 0;
				
			$records = array();
	
			while($row = pg_fetch_row($rs)){
				array_push($records,$row[0]);
			}
	
			return $records;
	
	}
	
	static function generateSelectColumns($columns){
			
		$string = "";
			
		$flag = 0;
		
		foreach($columns as $element){
				
			if(!($element['type'] == 'action')){
				if($flag == 0)
					$string .= $element['column'];
				else
					$string .= ",{$element['column']}";
	
				$flag++;
			}
	
		}

		if($string == "")
			return "*";
			
		
		return $string;
			
	}
	
	static function generateUpadteString($list){
	
		$updateStr = '';
	
		$keys = array_keys($list);
	
		$values = array_values($list);
	
		$isFirst = true;
	
		for($i = 0; $i != count($keys); $i++){
			if($isFirst){
				$updateStr .= $keys[$i]." = ".$values[$i];
				$isFirst = false;
			}else
				$updateStr .= " ,".$keys[$i]." = ".$values[$i];
				
		}
	
		return $updateStr;
	
	}
	
	static function retrievePrimaryKey($schema,$table){
	
		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod) FROM pg_index, pg_class, pg_attribute WHERE pg_class.oid = '$schema.$table'::regclass AND indrelid = pg_class.oid AND pg_attribute.attrelid = pg_class.oid AND pg_attribute.attnum = any(pg_index.indkey) AND indisprimary";
	
		$rs = DatabaseContext::execute($sql);
			
		$list = array();
	
		$pk = '';
	
		while($row = pg_fetch_row($rs)){
		
			self::$pk = $row[0];
			
			return $row[0];
				
		}
	
	}
	
	static function retrieveViewPrimaryKey($table){
	
		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod) FROM pg_index, pg_class, pg_attribute WHERE pg_class.oid = '$table'::regclass AND indrelid = pg_class.oid AND pg_attribute.attrelid = pg_class.oid AND pg_attribute.attnum = any(pg_index.indkey) AND indisprimary";
	
		$rs = DatabaseContext::execute($sql);
			
		$list = array();
	
		while($row = pg_fetch_row($rs)){
	
			return $row[0];
	
		}
	
	}
	
	//utilizado em getCOlumns
	static function retrieveForeignKeys($schema,$table,$generics){
	//SELECT DISTINCT kcu.column_name,ccu.table_schema,ccu.table_name,ccu.column_name FROM information_schema.table_constraints AS tc JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name WHERE constraint_type = 'FOREIGN KEY' AND tc.table_name='moradia' AND tc.table_schema = 'pesquisa' AND kcu.column_name = 'piso'
		$sql = "SELECT DISTINCT kcu.column_name,ccu.table_schema,ccu.table_name,ccu.column_name FROM information_schema.table_constraints AS tc JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name WHERE constraint_type = 'FOREIGN KEY' AND tc.table_name='$table' AND tc.table_schema = '$schema' $generics";
	
		$rs = DatabaseContext::execute($sql);
	
		$list = array();
		
		while($row = pg_fetch_row($rs)){
	
			if(self::$pk == $row[0])
				continue;
			
			$list["{$row[0]}"]['schema'] = $row[1];
			$list["{$row[0]}"]['table'] = $row[2];
			$list["{$row[0]}"]['pk'] = $row[3];
			$list["{$row[0]}"]['data'] = self::retrieveForeignKeyValues($row[1],$row[2],$row[3],$row[0]);
		
		}
	
		return $list;
	
	}
	
	//utilizado em getCOlumns
	static function retrieveForeignKeyValues($schema,$table,$column,$foreignColumn){
	
		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod),attnum FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT oid FROM pg_class WHERE oid = '$schema.$table'::regclass) and pg_attribute.attnum > 0 and pg_attribute.ATTISDROPPED = 'f' and format_type(pg_attribute.atttypid, pg_attribute.atttypmod) <> 'geometry'";
		
		$rs = DatabaseContext::execute($sql);
		
		$flag = 0;
			
		$foreignTableData = array();
		
		while($row = pg_fetch_row($rs)){
		
			$foreignTableData[$flag]['column'] = $row[0] ;
				
			$flag ++;
		}
		
		//Kl Manager Only
		$sql = '';
		
		if($table=='nivel'){
			$sql = "SELECT nivel.id,nome || ' - ' || nivel.descricao FROM nivel,tipo_licenca WHERE tipo_licenca.id = id_tipo_licenca ORDER BY nome,ordem";
		}else if($table=='subnivel'){
			$sql = "SELECT subnivel.id,nome || ' - ' || nivel.descricao || ' - ' || subnivel.descricao FROM nivel,tipo_licenca,subnivel WHERE subnivel.id_nivel = nivel.id AND tipo_licenca.id = id_tipo_licenca ORDER BY nome,subnivel.ordem";
		}else
			$sql = "SELECT {$foreignTableData[0]['column']},{$foreignTableData[1]['column']} FROM $schema.$table ORDER BY {$foreignTableData[1]['column']}";
			
		//$sql = "SELECT {$foreignTableData[0]['column']},{$foreignTableData[1]['column']} FROM $schema.$table ORDER BY {$foreignTableData[1]['column']}";
	
		$rs = DatabaseContext::execute($sql);
	
		$list = array();
			
		$flag = 0;
	
		$label = 0;
		
		while($row = pg_fetch_row($rs)){
	
			$label = $table=='tipo_licenca_obra'?DAO::retrieveTableOnlyValueByQuery("SELECT obra.descricao||' - '||nome FROM tipo_licenca,tipo_licenca_obra,obra WHERE obra.id = id_obra  AND tipo_licenca_obra.id_tipo_licenca = tipo_licenca.id and tipo_licenca_obra.id = {$row[0]}"):$row[1];
			
			self::$fksValues[$foreignColumn][$row[0]] = $label;
			
			$list[$flag]['value'] = $row[0];
			//$list[$flag]['label'] = $row[1];
			
			//Kl Manager Only
			$list[$flag]['label'] = $label;
			
			$flag++;
		}
	
		return $list;
	
	}
	
	//utilizado em action tables
	static function retrieveForeignKey($schema,$table,$foreignSchema,$foreignTable){
	
		$sql = "SELECT DISTINCT kcu.column_name FROM information_schema.table_constraints AS tc JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name WHERE constraint_type = 'FOREIGN KEY' AND tc.table_name='$table' AND tc.table_schema = '$schema' AND ccu.table_schema = '$foreignSchema' AND ccu.table_name = '$foreignTable'";
	
		$rs = DatabaseContext::execute($sql);
	
		while($row = pg_fetch_row($rs)){
	
			return $row[0];
		}
	
		return "";
	
	}
	
	static function retrieveTableOnlyOneFieldValue($schema,$table,$field, $pk,$id){
	
		$clause = $pk?" where $pk = $id":'';
		
		$sql = "select $field from $schema.$table $clause";
	
		$rs = DatabaseContext::execute($sql);
			
		$list = array();
		
		while($row = pg_fetch_row($rs)){
			return $row[0];
		}

		return "No Data";
		
	}
	
	static function retrieveTableOnlyValueByQuery($sql){
	
		$rs = DatabaseContext::execute($sql);
			
		$list = array();
	
		while($row = pg_fetch_row($rs)){
			return $row[0];
		}
	
		return 0;
	
	}
	
	static function retrieveValuesByQuery($sql){
	
		$rs = DatabaseContext::execute($sql);
			
		$list = array();
	
		while($row = pg_fetch_assoc($rs)){
			array_push($list, $row);
		}
	
		return $list;
	
	}
	
	//GIS
	
	static function retrieveGeomColumn($schema,$table){
	
		$sql = "SELECT f_geometry_column FROM geometry_columns WHERE f_table_schema = '$schema' AND f_table_name = '$table'";
	
		$rs = DatabaseContext::execute($sql);
	
		while($row = pg_fetch_row($rs)){
			return $row[0];
		}
	
	}
	
	static function retrieveNearestObject($schema,$table,$pk,$lat,$long){
	
		$geomColumn = self::retrieveGeomColumn($schema, $table);
	
		$sql = "select $pk from ctgis.vector where $geomColumn is not null ORDER BY st_distance((select st_setsrid(st_makepoint($long ,$lat),900913)),$geomColumn) ASC LIMIT 1";
	
		$rs = DatabaseContext::execute($sql);
		
		while($row = pg_fetch_row($rs)){
			return $row[0];
		}
		
		return "";
		
	}
	
	static function formatDataToUserFormat($data,$type){
	
		if(strstr($type, 'date')){
			
			$data = explode("-",$data);
			if(isset($data[1]))	
				return "{$data[2]}/{$data[1]}/{$data[0]}";
			else 
				return "";
		}
		else{

			$data = explode(" ",$data);
			
			$hour = $data[1];
			
			$data = explode("-",$data[0]);
			
			if(isset($data[1]))
				return "{$data[2]}/{$data[1]}/{$data[0]} $hour";
			else
				return "";
			
		}
		
		
	
	}
	
	static function retrieveGISRecords($schema,$table,$params){
	
		$cols 	= self::retrieveGISColumns($schema,$table,'');
	
		$select = self::generateSelectColumns($cols);
	
		$geom 	= self::retrieveGeomColumn($schema, $table);
	
		//echo "$select ---- $geom";
	
		$select = str_replace("$geom", "st_astext(st_centroid($geom))", $select);
	
		$sql = "SELECT $select FROM $schema.$table  WHERE $params";
		
		$rs = DatabaseContext::execute($sql);
	
		$flag = 0;
			
		$list = array();
	
		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){
				$list[$flag][$c]['column'] = $cols[$c]['column'];
				$list[$flag][$c]['label'] =
				isset(DataBase::$labels[$table])
				    &&isset(DataBase::$labels[$table][$cols[$c]['column']])?
				        DataBase::$labels[$table][$cols[$c]['column']]
				        :ucwords(str_replace("_"," ",$cols[$c]['column']));
				$list[$flag][$c]['type'] = $cols[$c]['type'];
	
				$list[$flag][$c]['isHidden'] = $cols[$c]['isHidden'];
	
				if($list[$flag][$c]['column'] != $geom)
					   $list[$flag][$c]['value'] = $row[$c];
					else
						$list[$flag][$c]['value'] = str_replace(')', '',str_replace('POINT(', '', $row[$c]));
			}
			$flag ++;
		}
	
		return $list;
	
	}
	
	static function retrieveGISColumns($schema,$table,$clauses){
	
		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod),attnum FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT oid FROM pg_class WHERE oid = '$schema.$table'::regclass) and pg_attribute.attnum > 0 and pg_attribute.ATTISDROPPED = 'f' $clauses";
	
		$rs = DatabaseContext::execute($sql);
	
		$flag = 0;
			
		$list = array();
	
		while($row = pg_fetch_row($rs)){
	
			$list[$flag]['column'] = $row[0] ;
			$list[$flag]['type'] = $row[1];
			$list[$flag]['id'] = $row[2];
			//	$prefix = explode("_", $row[0]);
			$list[$flag]['label'] = isset(DataBase::$labels[$table])&&isset(DataBase::$labels[$table][$row[0]])?DataBase::$labels[$table][$row[0]]:ucwords(str_replace("_"," ",$row[0]));
			$list[$flag]['isHidden'] = $row[0]=='id' || strstr($row[0],'id_');
			
			$flag ++;
		}
	
		//self::checkViewForm($schema, $table);
	
		//$list = self::retrieveColumnLabels($schema,$table,$list);
	
		return $list;
	
	}
	
	//SEC
	
	static function retrieveUser($login, $pass){
	
		$user = array();
	
		$sql = "SELECT id,nome FROM seguranca.usuario where login = '$login' AND senha = '".base64_encode($pass)."'";
		
		$rs = DatabaseContext::execute($sql);
	
		while($row = pg_fetch_row($rs)){
			//echo "$sql";
			$user['id'] = $row[0];
			$user['nome'] = $row[1];
				
		}
	
		if(!isset($user['id']))
			return false;
		
		return $user;
	
	}
	
	static function makeNearestObjectClause($schema,$table,$lat,$long){
	    
	    $geomColumn = self::retrieveGeomColumn($schema, $table);
	    
	    return "$geomColumn is not null ORDER BY st_distance((select st_setsrid(st_makepoint($long ,$lat),900913)),$geomColumn) ASC LIMIT 1";
	    
	}
	
	static function retrieveTitles($schema, $table, $cols){
	    
	    $list = array();
	    
	    $cols = explode(',',$cols);
	    
	    foreach($cols as $col){
	        //	$prefix = explode("_", $col);
	        $list[$col] = isset(DataBase::$labels[$schema.'.'.$table]) && isset(DataBase::$labels[$schema.'.'.$table][$col]) ? DataBase::$labels[$schema.'.'.$table][$col] : ucwords(str_replace("_"," ",$col));
	    }
	    
	    return $list;
	    
	}
	
	static function retrieveReportData($schema,$table, $cols, $params){
	    
	    if($params != '')
	        $params = " WHERE $params";
	        
	        $keys = explode(',',$cols);
	        
	        $select = $cols;
	        
	        $sql = "SELECT $select FROM $schema.$table $params";
	        
	        //	echo mb_convert_encoding($sql,'UTF-8', 'ISO-8859-1');
	        
	        $rs = DatabaseContext::execute($sql);
	        
	        $flag = 0;
	        
	        $records = array();
	        
	        while($row = pg_fetch_row($rs)){
	            for($c = 0; $c != count($keys); $c++){
	                
	                $records[$flag][$c]['column'] = $keys[$c] ;
	                $records[$flag][$c]['value'] = $row[$c];
	                
	            }
	            $flag ++;
	        }
	        
	        return $records;
	        
	}
	
	static function retrieveAutoCompleteRecords($schema,$table,$attrib,$params){
	    
	    
	    $sql = "SELECT DISTINCT $attrib FROM $schema.$table  WHERE $params LIMIT 5";
	    
	    $rs = DatabaseContext::execute($sql);
	    
	    $flag = 0;
	    
	    $list = array();
	    
	    while($row = pg_fetch_row($rs)){
	        
	        $list[$flag] = $row[0];
	        
	        $flag ++;
	        
	    }
	    
	    return $list;
	    
	}

}
?>