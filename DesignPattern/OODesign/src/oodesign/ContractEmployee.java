package d01tut1;

class ContractEmployee extends Employee {	
	public ContractEmployee(double salary) {
		super(salary);
	}

	public double getMonthlyPayExpense() {
		return getMonthlySalary();
	}
}