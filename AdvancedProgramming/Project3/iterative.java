import java.io.File;
import java.io.FileNotFoundException;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.NoSuchElementException;
// import java.util.Queue;
import java.util.Scanner;
import java.util.Stack;

public class iterative {
    public static void main(String[] args)
    {
        BinarySearchTree binaryTree = new BinarySearchTree();

        try
        {
            if (args.length < 1)
            {
                System.out.println("error - no file name given");
                System.out.println("\tusage: $java iterative filename");
                System.exit(0);
            }
            
            File inputFile = new File(args[0]);
            Scanner input = new Scanner(inputFile);
            while (input.hasNextLine())
            {
                String data = input.nextLine();
                if (data != "")
                    binaryTree.insert(data);
            }

            input.close();
        }
        catch (FileNotFoundException e)
        {
            System.out.println("ERROR - File not found");
            e.printStackTrace();
        }

        // System.out.println("\nThe tree data by level order: ");
        // binaryTree.printAll();

        System.out.println("\n");

        System.out.println("preorder traversal: ");
        binaryTree.printPreorder();
        
        System.out.println("inorder traversal: ");
        binaryTree.printInorder();

        System.out.println("postorder traversal: ");
        binaryTree.printPostorder();
    }
}

class BinarySearchTree
{
    private Node root;

    private class Node
    {
        String val;
        Node left_child, right_child;

        Node(String v)
        {
            this.val = v;
            this.left_child = this.right_child = null;
        }
    }

    public BinarySearchTree()
    {
        this.root = null;
    }

    public BinarySearchTree(String s)
    {
        this.root = new Node(s);
    }

    public void insert(String s)
    {
        if (this.root == null)
            this.root = new Node(s);
        else
        {
            Node current_node = this.root;

            while(true) {
                if (s.compareTo(current_node.val) < 0)
                {
                    if (current_node.left_child != null)
                        current_node = current_node.left_child;
                    else
                    {
                        current_node.left_child = new Node(s);
                        break;
                    }
                }
                else if (s.compareTo(current_node.val) > 0)
                {
                    if (current_node.right_child != null)
                        current_node = current_node.right_child;
                    else
                    {
                        current_node.right_child = new Node(s);
                        break;
                    }
                }
                else
                    break;
            }
        }
    }

    public void printPreorder()
    {
        TreeIterator iter = new TreeIterator("preorder");
        while(iter.hasNext())
            System.out.println("\t" + iter.next());
        System.out.println();
    }

    public void printInorder()
    {
        TreeIterator iter = new TreeIterator("inorder");
        while(iter.hasNext())
            System.out.println("\t" + iter.next());
        System.out.println();
    }

    public void printPostorder()
    {
        TreeIterator iter = new TreeIterator("postorder");

        while(iter.hasNext())
            System.out.println("\t" + iter.next());
        System.out.println();
    }

    private class TreeIterator implements Iterator<String>
    {
        Stack<Node> stack = new Stack<Node>();
        LinkedList<Node> visited;

        int traversal_type;

        public TreeIterator(String type)
        {
            switch(type.toUpperCase())
            {
                case "PREORDER":
                    this.traversal_type = 0;

                    if (root != null)
                        this.stack.push(root);

                    break;
                case "INORDER":
                    this.traversal_type = 1;

                    if (root != null)
                    {
                        this.stack.push(root);

                        Node temp_node = root;

                        while(temp_node.left_child != null)
                        {
                            temp_node = temp_node.left_child;
                            this.stack.push(temp_node);
                        }
                    }

                    break;
                case "POSTORDER":
                    this.traversal_type = 2;
                    this.visited = new LinkedList<Node>();

                    if (root != null)
                    {
                        this.stack.push(root);

                        Node temp_node = root;

                        while(temp_node.left_child != null)
                        {
                            temp_node = temp_node.left_child;
                            this.stack.push(temp_node);
                        }
                    }

                    break;
                default:
                    this.traversal_type = 0;
                    if (root != null)
                        this.stack.push(root);
            }
        }

        public boolean hasNext() { return !stack.isEmpty(); }
        
        public void remove()
        {
            throw new UnsupportedOperationException();
        }
        
        public String next()
        {
            if (!hasNext())
                throw new NoSuchElementException();
            
            Node current_node = this.stack.pop();

            switch (traversal_type)
            {
                case 0:
                    if (current_node.right_child != null)
                        this.stack.push(current_node.right_child);
                    if (current_node.left_child != null)
                        this.stack.push(current_node.left_child);

                    break;
                case 1:
                    if (current_node.right_child != null)
                    {
                        Node temp_node = current_node.right_child;
                        this.stack.push(temp_node);

                        while (temp_node.left_child != null)
                        {
                            temp_node = temp_node.left_child;
                            this.stack.push(temp_node);
                        }
                    }
                    break;
                case 2:
                    while (this.visited.contains(current_node))
                    {
                        current_node = this.stack.pop();
                    }

                    if (current_node.right_child != null && !this.visited.contains(current_node.right_child))
                    {
                        this.stack.push(current_node);

                        Node temp_node = current_node.right_child;

                        while (true)
                        {
                            if (temp_node.left_child != null)
                            {
                                this.stack.push(temp_node);

                                while (temp_node.left_child != null)
                                {
                                    temp_node = temp_node.left_child;
                                    this.stack.push(temp_node);
                                }
                            }
                            else
                            {
                                if (temp_node.right_child != null && !this.visited.contains(temp_node.right_child))
                                    temp_node = temp_node.right_child;
                                else
                                    break;
                            }
                        }

                        this.visited.add(temp_node);
                        return temp_node.val;
                    }
                    else
                        this.visited.add(current_node);
                    break;
                default:
                    throw new NoSuchElementException();
            }

            return current_node.val;
        }
    }

    // public void printAll()
    // {
    //     Queue<Node> list = new LinkedList<Node>();
    //     list.add(root);

    //     while (list.peek() != null)
    //     {
    //         Node current_node = list.poll();
    //         System.out.print("\nnode: " + current_node.val);

    //         if (current_node.left_child != null)
    //         {
    //             list.add(current_node.left_child);
    //             System.out.print("\n\tleft child: " + current_node.left_child.val);
    //         }
                
    //         if (current_node.right_child != null)
    //         {
    //             list.add(current_node.right_child);
    //             System.out.println("\n\tright child: " + current_node.right_child.val);
    //         }
    //     }
    // }
}