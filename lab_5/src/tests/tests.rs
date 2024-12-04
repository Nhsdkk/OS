use crate::btree::btree::{Node, NodeType};

#[test]
fn should_create_tree_correctly() {
    let mut root_node = Node::new(4, NodeType::ROOT);
    let l_node = Node::new(5, NodeType::LEFT);
    let r_node = Node::new(6, NodeType::RIGHT);
    root_node.attach_node(l_node);
    root_node.get_left().unwrap().borrow_mut().attach_node(r_node);
    assert_eq!(*root_node.get_data(), 4);
    assert!(root_node.get_right().is_none());
    assert!(root_node.get_left().is_some());

    let n1 = root_node.get_left().unwrap().borrow();

    assert_eq!(*n1.get_data(), 5);
    assert!(n1.get_left().is_none());
    assert!(n1.get_right().is_some());

    let n2 = n1.get_right().unwrap().borrow();

    assert_eq!(*n2.get_data(), 6);
    assert!(n2.get_right().is_none());
    assert!(n2.get_left().is_none());
}