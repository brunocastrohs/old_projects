const createTitle = (title)=>{
    return (<span><h5>{title}</h5><br/></span>);
}

const createRecord = (id, obj)=>{
    let body = [];

    for (let key in obj){
        let value = obj[key];
        key = key.toUpperCase().replaceAll("_", " ");
        body.push( (<li className="list-group-item" key={id+':'+key}> <span className='FeatureField'>{key}</span>: {value}</li>));
    }

    return <ul key={id+'ul'} className="FeatureInforRecord list-group">{body}</ul>;
}

export {createTitle, createRecord};