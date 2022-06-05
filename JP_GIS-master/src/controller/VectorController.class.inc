<?php

include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/config/Vector.class.inc");

class VectorController
{

    static $legendOptions = "";

    static $geoserverHost = Vector::HOST;
    
    static $containers = Vector::CONTAINERS;

    static $layers = Vector::LAYERS;

    static function initLayers()
    {
        
        $i = 0;
        
        for($i=0; $i < count(self::$layers);$i++){
            self::$layers[$i]['gid'] = $i+1;
            self::$layers[$i]['url'] = Vector::HOST.Vector::DS.'wms';
        }
        
    }

    static function getLayerSwitcher()
    {
        self::initLayers();
        
        $itensListComponent = "";
        
        foreach (self::$containers as $container) {
            // $gid com valor 4 devido as cmadas google
            $gid = 1;
            $itensListComponent .= "<li class='container' id='container{$container['gid']}T' onclick='Listener.collapseBranch(\"container{$container['gid']}\")'>{$container['name']}</li><ul id='container{$container['gid']}' class='layers'>";
            
            foreach (self::$layers as $layer) {
                
                if ($layer['container'] == $container['gid']) {
                    $itensListComponent .= "<li>" . HTML::createFormInput("checkbox", $layer['name'] . $layer['gid'], $layer['title'], "", 6, ($layer['visibility'] == 'true' ? " checked " : "  ") . (" onclick='VectorAction.setLayerVisibility($gid)'"));
                    $itensListComponent .= isset($layer['queryable']) ? self::getQueryComponent($layer, $gid) : "";
                    $itensListComponent .= isset($layer['description']) ? self::getDescriptionComponent($layer) : "";
                    $itensListComponent .= isset($layer['legend']) ? self::getLegendComponent($layer, $gid) : "";
                    $itensListComponent .= "</li>";
                }
                $gid ++;
            }
            
            $itensListComponent .= "</ul><br/>";
            $gid = 4;
        }
        
        // VECTOR
        // $itensListComponent .= "<li>" . HTML::createFormInput ( "checkbox", 'vec1', 'vec1', 'Vetores', "", 6, (" onclick='VectorAction.setLayerVisibility($gid)'") ) . "</li>";
        
        // GOOGLE
        $itensListComponent .= self::getGoogleBranch();
        
        return $itensListComponent;
    }

    static function getLegendComponent($layer, $gid)
    {
        $itensListComponent = HTML::createHREF("Visualizar Legenda", "#", "onclick='VectorAction.showMetaLegend(\"{$layer ['name'] }\", this)' class='metaLayer' ");
        $filterBoxes = self::generateLegendFilter($layer['schema'], $layer['name'], $layer['legend'], $gid);
        // $img = HTML::createIMG(self::getLegendImage($layer ['name']), "Legenda", "class='legendIMG'");
        $img = self::getLegendImage($layer['name']);
        $content = $filterBoxes;
        $content .= HTML::createFormInput('hidden', $layer['name'] . "LGS", '', 1, 0, "style='display:none'"); // HTML::createSPAN($layer ['name'] . "LGS", 0, "style='display:none'");
        $itensListComponent .= HTML::createSimpleDIV($layer['name'] . "LG", $content, "class='layerLegend' style=\"background:no-repeat url('$img') 25px 0%;\"");
        return $itensListComponent;
    }

    static function generateLegendFilter($schema, $table, $column, $layerGID)
    {
        $records = DAO::retrieveColumnValues($schema, $table, $column);
        
        $boxes = "";
        
        $id = 1;
        
        self::$legendOptions .= "{$table}L: [";
        
        foreach ($records as $e) {
            $boxes .= HTML::createLegendCheckbox( "{$table}L{$id}", "", $e, "2", " class='{$table}L' onclick='VectorAction.setLegendFilter(\"$column\",this,$layerGID)'");
            self::$legendOptions .= "'{$table}L{$id}',";
            $id ++;
        }
        
        self::$legendOptions .= "],";
        
        return $boxes;
    }

    static function getLegendImage($layer)
    {
        $workspace = "jp_gis";
        $imgURL = self::$geoserverHost . "/wms?REQUEST=GetLegendGraphic&VERSION=1.0.0&FORMAT=image/png&WIDTH=20&HEIGHT=21&legend_options=fontName:Arial;fontColor:0x000033;fontSize:11;bgColor:0xFFFFEE;&LAYER={$workspace}:{$layer}";
        return $imgURL;
    }

    static function getDescriptionComponent($layer)
    {
        $itensListComponent = HTML::createHREF("Visualizar Descrição", "#", "onclick='VectorAction.showMetaDescription(\"{$layer ['name'] }\", this)' class='metaLayer' ");
        $content = HTML::createFormInput('hidden', $layer['name'] . "DSS", '', 1, 0, "style='display:none'"); // HTML::createSPAN($layer ['name'] . "DSS", 0, "style='display:none'");
        $itensListComponent .= HTML::createSimpleDIV($layer['name'] . "DS", $layer['description'] . $content, "class='layerDescription'");
        return $itensListComponent;
    }
    
    static function getQueryComponent($layer, $gid)
    {
        $columns = DAO::retrieveColumns($layer['schema'], $layer['name'], "");
        $itensListComponent = HTML::createHREF("Visualizar Pesquisador", "#", "onclick='VectorAction.showMetaQuery(\"{$layer ['name'] }\", this)' class='metaLayer' ");
        $content  = HTML::createFormInput('hidden', $layer['name'] . "QSS", '', 1, 0, "style='display:none'");
        $content .= self::createQueryComboBox($layer['name'], "Filtro", $columns, "style='font-size:11px;'", $layer, $gid);
        
        
        $itensListComponent .= HTML::createSimpleDIV($layer['name'] . "QS", $content, "class='layerDescription'");
        return $itensListComponent;
    }
    
    static function createQueryComboBox($inputID,$inputLabel,$data,$generics, $layer, $gid){
        
        $attribs = self::createAttribComboBox($data, $layer, $generics);
        
        $operators = self::createOperatorComboBox($inputID, $generics);
        
        $logic = self::createLogicComboBox($inputID, $generics);
        
        $input = "<div class='12u$' id='$inputID.KeyContainer'>";
        $input .= "<input name='$inputID.Key' id='$inputID.Key' onkeyup='VectorAction.autoComplete($gid,this)' placeholder='Digite um valor...' type='text' style='font-size:11px;'>";
        $input .= "<div class='12u$ autoCompleteBox' id='$inputID.AutoCompleteKey'></div>";
        $input .= "</div>";
        
        
        $button = "<div class='12u$'>
                    <input class='5u$' value='Filtrar' onclick='VectorAction.setQueryFilter(\"{$layer['name']}\",$gid)' name='$inputID.Button' id='$inputID.Button' type='button' style='font-size:11px; margin: 0 0 0 12px; padding:0 0 0 0; '/>
                    <input class='5u$' value='Adicionar' onclick='VectorAction.pushNewFilter(\"{$layer['name']}\",$gid)' name='$inputID.Button' id='$inputID.Button' type='button' style='font-size:11px; margin: 0 0 0 12px; padding:0 0 0 0; '/>
                    <br/><br/>
                    <input class='5u$' value='Resetar' onclick='VectorAction.resetQueryFilter(\"{$layer['name']}\",$gid)' name='$inputID.Reset' id='$inputID.Reset' type='button' style='font-size:11px; margin: 0 0 0 12px; padding:0 0 0 0;'/>
                    <input class='5u$' value='Exportar' onclick='VectorAction.exportData(\"{$layer['name']}\",$gid)' name='$inputID.Reset' id='$inputID.Reset' type='button' style='font-size:11px; margin: 0 0 0 12px; padding:0 0 0 0;'/>
                    </div>";
        
        return $logic.
        "<br/>".$attribs.
        "<br/>".$operators.
        "<br/>".$input.
        "<br/><div id='multiFilter.$inputID' style='width:100%;'></div>".
        $button
        ;
        
    }
    
    static function createLogicComboBox($inputID, $generics){
        
        $options = "<option value=' OR '>OU</option>
                    <option value=' AND '>E</option>
                    ";
        
        return "<div id='$inputID.LogicDropdownContainer' style='display:none' class='12u$'><div class='select-wrapper'><select name='$inputID.LogicDropdown' id='$inputID.LogicDropdown' $generics><option value=''>Selecione um conectivo...</option>$options</select></div></div>";
        
    }
    
    static function createOperatorComboBox($inputID, $generics){
        
        $options = "<option value='='>Igual a</option>
                    <option value='>'>Maior que</option>
                    <option value='>='>Maior ou igual a</option>
                    <option value='<'>Menor que</option>
                    <option value='<='>Menor ou igual a</option>
                    <option value='<>'>Diferente</option>
                    <option value='ilike'>Contém</option>
                    ";
        
        return "<div id='$inputID.OperatorDropdownContainer' class='12u$'><div class='select-wrapper'><select name='$inputID.OperatorDropdown' id='$inputID.OperatorDropdown' $generics><option value=''>Selecione um operador...</option>$options</select></div></div>";
        
    }
    
    static function createAttribComboBox($data, $layer, $generics){
        
        
        
        $options = "";
        
        if($data)
            foreach($data as $object){
                $key = $layer['schema'].'.'.$layer['name'].'.'.$object['column'];
                if(!isset(DataBase::$hiddenColumns[$key]) && $object['column'] != 'gid' && $object['column'] != 'geom')
                    $options .= self::createOption($object['column'].'!!'.$object['type'], $object['label'],'') ;
        }
        
        return "<div id='{$layer['name']}.AttribContainer' class='12u$'  id='row.{$layer['name']}'><div class='select-wrapper'><select name='{$layer['name']}.AttribDropdown' id='{$layer['name']}.AttribDropdown' $generics><option value=''>Selecione um atríbuto...</option>$options</select></div></div>";
        
    }
    
    static  function createOption($value,$text,$generics){
        
        return "<option $generics value='$value'>$text</option>";
        
    }

    static function getGoogleBranch()
    {
        $itensListComponent = "<li class='container' id='containerGT' onclick='Listener.collapseBranch(\"containerG\")'>Open Street Maps</li> <ul id='containerG' class='layers'>";
        $itensListComponent .= "<li>" . HTML::createFormInput("checkbox", 'g1', 'Open Street Maps (OSM)', "", 6, ("checked onclick='VectorAction.setLayerVisibility(0)'")) . "</li>";
        $itensListComponent .= "</ul>";
        return $itensListComponent;
    }

    static function getLayerTitle($str)
    {
        /*foreach (self::$layers as $layer) {
            if ($str == $layer['name'])
                return $layer['title'];
        }*/
        
        return '';
    }

    static function getOlLayers()
    {
        
        static $layerCount = 1;
        $str = "[";
        $gid = 0;
        
        foreach (self::$layers as $layer) {
            $str .= "{'gid':{$gid},
					'name':'{$layer['name']}',
					'title':'{$layer['title']}',
					'url':'{$layer['url']}',
					'visibility':{$layer['visibility']},
					'group':'{$layer['group']}',
					'schema':'{$layer['schema']}',
					'container':'{$layer['container']}',
					'view':'{$layer['view']}',
					'consultable':".(isset($layer['consultable'])?$layer['consultable']:'false')."},";
            $gid ++;
        }
        
        $str .= "]";
        
        return $str;
    }

    static function writeVectors()
    {
        if (SecurityUtil::checkSession()) {
            $rows = self::retrieveVectors($_SESSION['user_ufcmanager']['id']);
            
            $strVectors = '';
            
            HTML::generateJsScript("VectorManager.vectorCount = " . (DAO::retrieveIdCount('ctgis', 'vector', 'id') + 1) . ";");
            
            foreach ($rows as $row) {
                
                $strVectors .= "{wkt:'{$row['geom']}',id:{$row['id']},usuario:'{$row['usuario']}',tipo:'{$row['tipo']}',id_projeto:'{$row['id_projeto']}',id_obra:'{$row['id_obra']}'},";
            }
            
            HTML::generateJsScript("VectorManager.initialFeatures = [$strVectors]");
        }
    }

    static function initGIS()
    {
        $layers = self::getOlLayers();
        
        $legendOptions = self::$legendOptions;
        
        $bounds = Vector::BOUNDS;
        
        $projection = Vector::PROJECTION;
        
        $center = Vector::CENTER;
        
        $zoom = Vector::ZOOM;
        
        echo HTML::createSPAN('vectorMap', HTML::createSPAN('mousePosition', "", ""), ' style="margin-top:0px;" ');
        HTML::generateJsScript("VectorManager.bounds = new OpenLayers.Bounds($bounds); ");
        //HTML::generateJsScript("VectorManager.extent = new OpenLayers.Bounds($extent); ");
        HTML::generateJsScript("VectorManager.projection = '$projection' ;");
        HTML::generateJsScript("VectorManager.zoom = '$zoom' ;");
        HTML::generateJsScript("VectorManager.center =  $center; ");
        HTML::generateJsScript("VectorManager.layers = $layers;");
        HTML::generateJsScript("VectorAction.legendOptions = { $legendOptions };");
        HTML::generateJsScript("VectorManager.initGIS();");
    }
}

?>