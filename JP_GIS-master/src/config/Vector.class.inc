<?php

class Vector {

    public const HOST = 'http://localhost:8080/geoserver';
    
    public const DS = '/jp_gis/';
    
    public const CONTAINERS = array(
        array(
            'gid' => 1,
            'name' => 'Limites',
            "visibility" => true
        ),
        
        array(
            'gid' => 2,
            'name' => 'Infra.',
            "visibility" => true
        ),
        
        array(
            'gid' => 3,
            'name' => 'Ambiental',
            "visibility" => true
        ),
        
        array(
            'gid' => 4,
            'name' => 'Topografia',
            "visibility" => true
        )
        
        ,
        
        array(
            'gid' => 5,
            'name' => 'Imagem',
            "visibility" => true
        )
    );
    
    public const LAYERS = array(
        
        array(
            'gid' => 0,
            'name' => 'JP_reprocessado',
            'title' => 'Imagem de satelite',
            'url' => '',
            'visibility' => 'true',
            'group' => 'raster',
            'schema' => 'public',
            'container' => 5,
            // 'legend' => 'intervalo',
            'view' => 'false',
            //'consultable' => 'false',
            //'queryable' => 'false',
        ),
        
        array(
            'gid' => 0,
            'name' => 'area_urbanizada',
            'title' => 'Área Urbanizada',
            'url' => '',
            'visibility' => 'true',
            'group' => 'polygon',
            'schema' => 'public',
            'container' => 1,
            // 'legend' => 'intervalo',
            'view' => 'false',
            //'consultable' => 'false',
            //'queryable' => 'false',
        ),
        
        array(
            'gid' => 0,
            'name' => 'bairro',
            'title' => 'Bairros',
            'url' => '',
            'visibility' => 'false',
            'group' => 'polygon',
            'schema' => 'public',
            'container' => 1,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'setor',
            'title' => 'Setor',
            'url' => '',
            'visibility' => 'false',
            'group' => 'point',
            'schema' => 'public',
            'container' => 1,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'lote',
            'title' => 'Lote',
            'url' => '',
            'visibility' => 'false',
            'group' => 'point',
            'schema' => 'public',
            'container' => 1,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'telhado',
            'title' => 'Telhado',
            'url' => '',
            'visibility' => 'false',
            'group' => 'point',
            'schema' => 'public',
            'container' => 1,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'curva',
            'title' => 'Curvas',
            'url' => '',
            'visibility' => 'false',
            'group' => 'line',
            'schema' => 'public',
            'container' => 3,
            // 'legend' => 'intervalo',
            'view' => 'false',
            //'consultable' => 'false',
            //'queryable' => 'false',
        ),
        
        array(
            'gid' => 0,
            'name' => 'estrada',
            'title' => 'Estradas',
            'url' => '',
            'visibility' => 'false',
            'group' => 'line',
            'schema' => 'public',
            'container' => 2,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'hidrografia',
            'title' => 'Hidrografia',
            'url' => '',
            'visibility' => 'false',
            'group' => 'polygon',
            'schema' => 'public',
            'container' => 3,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'marco_topografico',
            'title' => 'Marcos Topográficos',
            'url' => '',
            'visibility' => 'false',
            'group' => 'point',
            'schema' => 'public',
            'container' => 4,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'mask',
            'title' => 'Máscara',
            'url' => '',
            'visibility' => 'false',
            'group' => 'polygon',
            'schema' => 'public',
            'container' => 4,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'ponte',
            'title' => 'Pontes',
            'url' => '',
            'visibility' => 'false',
            'group' => 'line',
            'schema' => 'public',
            'container' => 2,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'praca',
            'title' => 'Praça',
            'url' => '',
            'visibility' => 'false',
            'group' => 'polygon',
            'schema' => 'public',
            'container' => 2,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        array(
            'gid' => 0,
            'name' => 'servico',
            'title' => 'Serviços',
            'url' => '',
            'visibility' => 'false',
            'group' => 'point',
            'schema' => 'public',
            'container' => 4,
            // 'legend' => 'intervalo',
            'view' => 'false',
            'consultable' => 'true',
            'queryable' => 'true',
        ),
        
        
    );
    
    public const BOUNDS = '-4697991.5, -391495.28125, -4696193.5, -389176.28125';
    
    public const PROJECTION = "EPSG:900913";
    
    public const CENTER = '[-4697248.5, -391472.5]';
    
    public const ZOOM = '14';
    
}

?>