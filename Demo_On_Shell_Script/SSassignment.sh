#!/bin/bash

# Function to display the menu
function display_menu() {
    echo "Menu:"
    echo ""
    echo "1. Create Employee Record"
    echo "2. Delete Employee Record"
    echo "3. Search Employee Record"
    echo "4. Display Employee Details"
    echo "5. Sort Records"
    echo "6. List All Records"
    echo "7. Update Employee Designation"
    echo "8. Update Phone Number"
    echo "9. Update Department"
    echo "10. Update Salary"
    echo "11. Display All The Employee ID's"
    echo "12. Display All The Names"
    echo "13. Display All Departments"
    echo "14. Display all the Designations"
    echo "15. EXIT"
    echo ""
}



#Function to update the employee designations
function update_designations(){
         read -p "Enter the Employee Number: " chng_emp_no
         read -p "Enter the older designation: " old_designation
         read -p "Enter the new designation: " new_designation

	 #We are finding the row with the given emp_no and then using pipe we are giving the output to sed
         #Using sed we are replacing old designation to the new designation
	 #We are saving the result of sed into temp.txt
         grep "^.*$chng_emp_no:.*$" employee_records.txt | sed "s/$old_designation/$new_designation/g" employee_records.txt>temp.txt
	 
	 #we will move the content from temp back to employee_records.txt
         mv temp.txt employee_records.txt

         echo "Employee Designation Updated Successfully"
         echo ""
         echo "EmpId:Name:Designation:Department:Phone No:Salary"
         grep "^.*$chng_emp_no:.*$" employee_records.txt 
}



#Function to update the employee phone number
function update_phoneno(){
           read -p "Enter the Employee Number: " chng_emp_no
           read -p "Enter the older Phone Number: " old_number
           read -p "Enter the new Phone Number: " new_number
           grep "^.*$chng_emp_no:.*$" employee_records.txt | sed "s/$old_number/$new_number/g" employee_records.txt > temp.txt
           mv temp.txt employee_records.txt
           echo "Employee Phone Number Updated Successfully"
           echo ""
           echo "EmpId:Name:Designation:Department:Phone No:Salary"
           grep "^.*$chng_emp_no:.*$" employee_records.txt
}



#Function to update the employee department
function update_department(){
         read -p "Enter the Employee Number: " chng_emp_no
         read -p "Enter the older department: " old_department
         read -p "Enter the new department: " new_department
         grep "^.*$chng_emp_no:.*$" employee_records.txt | sed "s/$old_department/$new_department/g" employee_records.txt>temp.txt
         mv temp.txt employee_records.txt
         echo "Employee Department Updated Successfully"
         echo ""
         echo "EmpId:Name:Designation:Department:Phone No:Salary"
         grep "^.*$chng_emp_no:.*$" employee_records.txt
}



#Function to update the employee's salary
function update_salary(){
         read -p "Enter the Employee Number: " chng_emp_no
         read -p "Enter the older salary: " old_salary
         read -p "Enter the new salary: " new_salary
         grep "^.*$chng_emp_no:.*$" employee_records.txt | sed "s/$old_salary/$new_salary/g" employee_records.txt>temp.txt
         mv temp.txt employee_records.txt
         echo "Employee Salary Updated Successfully"
         echo ""
         echo "EmpId:Name:Designation:Department:Phone No:Salary"
         grep "^.*$chng_emp_no:.*$" employee_records.txt
}



#Function to display the employee ids
function display_empid(){
     awk -F ":" '{print $1}' employee_records.txt | sort | uniq
}



#Function to display the names
function display_names(){
     awk -F ":" '{print $2}' employee_records.txt | sort | uniq
}



#Function to display the column department
function display_departments(){
     awk -F ":" '{print $4}' employee_records.txt | sort | uniq
}



#Function to display the column designations 
function display_designations(){
     awk -F ":" '{print $3}' employee_records.txt | sort | uniq
}



# Function to create a new employee record
function create_record() {
    read -p "Enter Employee Number: " emp_number
    read -p "Enter Employee Name: " emp_name
    read -p "Enter Employee Designation: " emp_desig
    read -p "Enter Employee Department: " emp_dept
    read -p "Enter Employee Phone Number: " emp_phone
    read -p "Enter Employee Salary: " emp_sal
 
    # Check if the employee number already exists
    if grep -q "^.*:$emp_number:.*$" employee_records.txt; then
        echo "Employee Number $emp_number already exists. Cannot create duplicate records."
    else
        # Append the new record to the file
        echo "$emp_number:$emp_name:$emp_desig:$emp_dept:$emp_phone:$emp_sal" >> employee_records.txt
        echo "Employee Record created successfully!"
    fi
}



# Function to delete an employee record
function delete_record() {
    read -p "Enter the Employee Number to delete: " emp_number
    # Use temporary file to store the updated records
    grep -v "^.*$emp_number:.*$" employee_records.txt > temp.txt
    mv temp.txt employee_records.txt
    echo "Employee Record deleted successfully!"
}



# Function to search for an employee record
function search_record() {
    read -p "Enter the Employee Number to search: " emp_number
    echo ""
    echo "EmpId:Name:Designation:Department:Phone No:Salary"
    grep "^.*$emp_number:.*$" employee_records.txt 
}



# Function to display a specific employee's details
function display_employee_details() {
    search_record
}



# Function to sort the records
function sort_records() {
    sort -t ':' -k2n employee_records.txt > temp.txt
    mv temp.txt employee_records.txt
    echo "Record Sorted Successfully!!!"
}



# Function to list all records
function list_records() {
    awk 'BEGIN {FS=":";OFS=" "; print "Emp_Number\tName\tDesignation\tDepartment\tPhone_Number\tSalary";}
    {print $1,"\t",$2,"\t",$3,"\t",$4,"\t",$5,"\t"$NF;}' employee_records.txt
}

	

# Main program loop
while true; do
    display_menu
    read -p "Enter your choice (1-15): " choice

    case $choice in
        1)
            create_record
            ;;
        2)
            delete_record
            ;;
        3)
            search_record
            ;;
        4)
            display_employee_details
            ;;
        5)
            sort_records
            ;;
        6)
            list_records
            ;;
	7)
	    update_designations
	    ;;
	8)
	    update_phoneno
	    ;;
	9)
	    update_department
	    ;;
	10)
	    update_salary
	    ;;
	11)
	    display_empid
	    ;;
	12)
	    display_names
	    ;;
	13)
	   display_departments
	   ;;
	14)
	   display_designations
	   ;;	
        15)
            echo "Exiting..."
            exit 0
            ;;
        *)
            echo "Invalid choice. Please enter a valid option (1-15)."
            ;;
    esac

    echo # Add a new line for better formatting
done
