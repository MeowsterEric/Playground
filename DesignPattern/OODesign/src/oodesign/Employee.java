package d01tut1;

public abstract class Employee {
	private double salary;
	
	public Employee (double salary) {
		this.salary = salary;
	}
	
	public double getMonthlySalary() {
		return salary / 12;
	}
	
	protected double getSalary() {
		return this.salary;
	}
	
	public abstract double getMonthlyPayExpense();
}
