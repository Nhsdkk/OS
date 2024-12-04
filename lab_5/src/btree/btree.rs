use std::cell::RefCell;
use std::rc::Rc;

#[derive(PartialEq, Clone)]
pub enum NodeType {
    LEFT,
    RIGHT,
    ROOT
}

#[derive(Clone, PartialEq)]
pub struct Node<T> {
    data: T,
    node_type: NodeType,
    l_node: Option<Rc<RefCell<Node<T>>>>,
    r_node: Option<Rc<RefCell<Node<T>>>>
}

impl<T> Node<T> {
    pub fn new(data: T, node_type: NodeType) -> Node<T> {
        Node {
            data,
            node_type,
            l_node: None,
            r_node: None
        }
    }

    pub fn get_data(&self) -> &T{
        &self.data
    }

    pub fn attach_node(& mut self, node: Node<T>) {
        match node.node_type {
            NodeType::LEFT => {
                self.l_node = Some(Rc::new(RefCell::new(node)));
                return;
            },
            NodeType::RIGHT => {
                self.r_node = Some(Rc::new(RefCell::new(node)));
                return;
            },
            NodeType::ROOT => { panic!("Trying to insert root to subnode") }
        }
    }

    pub fn detach_node(& mut self, node_type : NodeType) {
        match node_type {
            NodeType::LEFT => {
                if self.l_node.is_none() {
                    panic!("Trying to remove none lnode");
                }

                self.l_node = None;
            }
            NodeType::RIGHT => {
                if self.r_node.is_none() {
                    panic!("Trying to remove none rnode");
                }

                self.r_node = None;
            }
            NodeType::ROOT => { panic!("Trying to remove root") }
        }
    }

    pub fn get_left(&self) -> Option<&Rc<RefCell<Node<T>>>> {
        if self.l_node.is_some() {
            let node = self.l_node.as_ref().unwrap();
            return Some(node);
        }

        None
    }

    pub fn get_right(&self) -> Option<&Rc<RefCell<Node<T>>>> {
        if self.r_node.is_some() {
            let node = self.r_node.as_ref().unwrap();
            return Some(node);
        }

        None
    }

    pub fn drop_node(&mut self, node_type : NodeType) {
        match node_type {
            NodeType::LEFT => { self.l_node = None; }
            NodeType::RIGHT => { self.r_node = None; }
            NodeType::ROOT => { return; }
        }
    }
}