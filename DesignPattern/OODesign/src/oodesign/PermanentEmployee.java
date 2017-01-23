package oodesign;

class PermanentEmployee extends Employee {

	private double vacationRate;

	public PermanentEmployee(double salary, double vacationRate) {
		super(salary);
		this.vacationRate = vacationRate;
	}

	public double vacationPay() {
		return getSalary() * vacationRate / 12;
	}

	public double getMonthlyPayExpense() {
		return getMonthlySalary() + vacationPay();
	}

}
