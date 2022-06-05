function geoCSVReader(resource, geoWorkspace, serverURL, serverType) {

    let tree = [];
    let layers = [];
    let rows = resource.split('\n');
    

    rows.map(row => {
        let data = row.split(';');
        let layerLevel = 1;
        let category_id = populateTree(tree, data[1], data[2]!=='');

        if (data[2]) {
            let major_class_id = populateTree(tree[category_id].subFolder, data[2], data[3]!=='');
            layerLevel++;
            if (data[3]) {
                let sub_class_id = populateTree(tree[category_id].subFolder[major_class_id].subFolder, data[3], data[4]!=='');
                layerLevel++;
                if (data[4]){
                    populateTree(tree[category_id].subFolder[major_class_id].subFolder[sub_class_id].subFolder, data[4], false);
                    layerLevel++;
                }
            }
            
        }

        layerInclude(layers, geoWorkspace + ':' + data[5].toLowerCase(), data[6], data[layerLevel], serverURL, serverType);
        return true;
    }
    );

    return { tree, layers };

}

function populateTree(object, title, hasSubFolder = true) {
    if (title && object && !object.find(e => e.title === title)) {
        object.push({ title, subFolder: hasSubFolder ? [] : null })
    }
    return object.findIndex(e => e.title === title) >= 0 ? object.findIndex(e => e.title === title) : 0;
}

function layerInclude(object, wmsName, title, folder, url, serverType) {
    object.push({ wmsName, title, folder, url, visibility: false, queryable: true, serverType })
}


export default geoCSVReader;