VectorStyle = {
	
	vectorStyles:'',
	obraStyles:'',
	projetoStyles:'',
	
	init : function() {
		
		var commonDefaultRule = new OpenLayers.Rule({
             symbolizer: {
                 "Point": {
                     pointRadius: 5,
                     graphicName: "square",
                     fillColor: "white",
                     fillOpacity: 0.25,
                     strokeWidth: 1,
                     strokeOpacity: 1,
                     strokeColor: "#3333aa"
                 },
                 "Line": {
                     strokeWidth: 3,
                     strokeOpacity: 1,
                     strokeColor: "#6666aa"
                 },
                 "Polygon": {
                     strokeWidth: 1,
                     strokeOpacity: 1,
                     fillColor: "#9999aa",
                     strokeColor: "#6666aa"
                 }
             }
         })
		
		var obraDefaultRule = new OpenLayers.Rule({
        	filter: new OpenLayers.Filter.Comparison({
      	      type: OpenLayers.Filter.Comparison.EQUAL_TO,
      	      property: "tipo",
      	      value: 2,
        	}),
        	symbolizer: {
              "Point": {
                  pointRadius: 5,
                  graphicName: "square",
                  fillColor: "white",
                  fillOpacity: 0.25,
                  strokeWidth: 1,
                  strokeOpacity: 1,
                  strokeColor: "#FF3B00"
              },
              "Line": {
                  strokeWidth: 3,
                  strokeOpacity: 1,
                  strokeColor: "#FF3B00"
              },
              "Polygon": {
                  strokeWidth: 1,
                  strokeOpacity: 1,
                  fillColor: "#FF703D",
                  strokeColor: "#FF3B00"
              }
          }
      });
		
	  var projetoDefaultRule = new OpenLayers.Rule({
        	filter: new OpenLayers.Filter.Comparison({
      	      type: OpenLayers.Filter.Comparison.EQUAL_TO,
      	      property: "tipo",
      	      value: 1,
        	}),
        	symbolizer: {
              "Point": {
                  pointRadius: 5,
                  graphicName: "square",
                  fillColor: "white",
                  fillOpacity: 0.25,
                  strokeWidth: 1,
                  strokeOpacity: 1,
                  strokeColor: "#000000"
              },
              "Line": {
                  strokeWidth: 3,
                  strokeOpacity: 1,
                  strokeColor: "#000000"
              },
              "Polygon": {
                  strokeWidth: 1,
                  strokeOpacity: 1,
                  fillColor: "#7A7A7A",
                  strokeColor: "#000000"
              }
          }
      });

	VectorStyle.vectorStyles = new OpenLayers.StyleMap({
            "default": new OpenLayers.Style(null, {
                rules: [
                    commonDefaultRule,
                    projetoDefaultRule,
                    obraDefaultRule
                    
                    
                ]
            }),
            "select": new OpenLayers.Style(null, {
                rules: [
                    new OpenLayers.Rule({
                        symbolizer: {
                            "Point": {
                                pointRadius: 5,
                                graphicName: "square",
                                fillColor: "white",
                                fillOpacity: 0.25,
                                strokeWidth: 2,
                                strokeOpacity: 1,
                                strokeColor: "#0000ff"
                            },
                            "Line": {
                                strokeWidth: 3,
                                strokeOpacity: 1,
                                strokeColor: "#0000ff"
                            },
                            "Polygon": {
                                strokeWidth: 2,
                                strokeOpacity: 1,
                                fillColor: "#0000ff",
                                strokeColor: "#0000ff"
                            }
                        }
                    })
                ]
            }),
            "temporary": new OpenLayers.Style(null, {
                rules: [
                    new OpenLayers.Rule({
                        symbolizer: {
                            "Point": {
                                graphicName: "square",
                                pointRadius: 5,
                                fillColor: "white",
                                fillOpacity: 0.25,
                                strokeWidth: 2,
                                strokeColor: "#0000ff"
                            },
                            "Line": {
                                strokeWidth: 3,
                                strokeOpacity: 1,
                                strokeColor: "#0000ff"
                            },
                            "Polygon": {
                                strokeWidth: 2,
                                strokeOpacity: 1,
                                strokeColor: "#0000ff",
                                fillColor: "#0000ff"
                            }
                        }
                    })
                ]
            })
        });

	},
	
};