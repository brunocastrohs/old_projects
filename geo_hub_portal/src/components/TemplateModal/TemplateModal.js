import './TemplateModal.css';
import React,{useState, useEffect} from 'react';
import {Button, Modal} from 'react-bootstrap';

function TemplateModal(props) {

  const [show, setShow] = useState(false);

  useEffect( () => {
    if(props.children && window.activeFillModal)
      handleShow();
  },[props.children])

  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  return (
      <Modal show={show} onHide={handleClose}>
        <Modal.Header>
          <Modal.Title>Informação</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          {props.children}
        </Modal.Body>
        <Modal.Footer>
          <Button variant="secondary" onClick={handleClose}>
            Fechar
          </Button>
          {/*<Button variant="primary" onClick={handleClose}>
            Save Changes
          </Button>*/}
        </Modal.Footer>
      </Modal>
  );

}

export default TemplateModal;