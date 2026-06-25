#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Structure used to store course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Tree node structure
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

public:
    BinarySearchTree() {
        root = nullptr;
    }

    void Insert(Course course);
    void InOrder();
    Course Search(string courseNumber);

private:
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
};

// Insert a course into the binary search tree
void BinarySearchTree::Insert(Course course) {

    // If the tree is empty, this course becomes the root
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

// Helper function that finds the correct place for a new course
void BinarySearchTree::addNode(Node* node, Course course) {

    // Smaller course numbers go to the left
    if (course.courseNumber < node->course.courseNumber) {

        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        // Larger course numbers go to the right
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

// Search for a course by course number
Course BinarySearchTree::Search(string courseNumber) {

    Node* currentNode = root;

    // Move through the tree until a match is found
    while (currentNode != nullptr) {

        if (currentNode->course.courseNumber == courseNumber) {
            return currentNode->course;
        }
        else if (courseNumber < currentNode->course.courseNumber) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }

    // Return an empty course if not found
    Course course;
    return course;
}

// Print all courses in alphanumeric order
void BinarySearchTree::InOrder() {

    inOrder(root);
}

// Helper function for in-order traversal
void BinarySearchTree::inOrder(Node* node) {

    if (node != nullptr) {

        // Left side prints first because it has smaller course numbers
        inOrder(node->left);

        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;

        // Right side prints after because it has larger course numbers
        inOrder(node->right);
    }
}

// Split a line of text into parts using commas
vector<string> splitLine(string line) {

    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    return tokens;
}

// Load courses from the CSV file into the binary search tree
void loadCourses(string fileName, BinarySearchTree* courseTree) {

    ifstream file(fileName);

    // Check that the file opened correctly
    if (!file.is_open()) {
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;

    while (getline(file, line)) {

        vector<string> tokens = splitLine(line);

        // Each line must have at least a course number and course title
        if (tokens.size() < 2) {
            cout << "Error: Invalid course data format." << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens.at(0);
        course.courseTitle = tokens.at(1);

        // Store any prerequisites listed after the title
        for (unsigned int i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens.at(i));
        }

        courseTree->Insert(course);
    }

    file.close();

    cout << "Data loaded successfully." << endl;
}

int main() {
    
	// Create the binary search tree used to store courses
    BinarySearchTree courseTree;
    string fileName;
    int userChoice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

	// Continue showing the menu until the user chooses to exit
    while (userChoice != 9) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << endl;

		// Get menu selection from the user
        cout << "What would you like to do? ";
        if (!(cin >> userChoice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

		// Option 1: Load course data from a CSV file
        if (userChoice == 1) {
            cout << "Enter file name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, fileName);

            loadCourses(fileName, &courseTree);
            dataLoaded = true;
        }

		// Option 2: Print all courses in alphanumeric order
        else if (userChoice == 2) {
            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl << endl;
                courseTree.InOrder();
            }
        }

		// Option 3: Search for and display a specific course
        else if (userChoice == 3) {
            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            }
            else {
                string courseNumber;

                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                Course course = courseTree.Search(courseNumber);

                if (course.courseNumber.empty()) {
                    cout << "Course not found." << endl;
                }
                else {
                    cout << course.courseNumber << ", " << course.courseTitle << endl;

                    cout << "Prerequisites: ";

                    if (course.prerequisites.empty()) {
                        cout << "None";
                    }
                    else {
                        cout << endl;

                        for (unsigned int i = 0; i < course.prerequisites.size(); ++i) {
                            string prerequisiteNumber = course.prerequisites.at(i);
                            Course prerequisiteCourse = courseTree.Search(prerequisiteNumber);

                            if (!prerequisiteCourse.courseNumber.empty()) {
                                cout << prerequisiteCourse.courseNumber << ", "
                                    << prerequisiteCourse.courseTitle << endl;
                            }
                            else {
                                cout << prerequisiteNumber << " not found" << endl;
                            }
                        }
                    }

                    cout << endl;
                }
            }
        }
        
		// Option 9: Exit the program
        else if (userChoice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << userChoice << " is not a valid option." << endl;
        }
    }

    return 0;
}