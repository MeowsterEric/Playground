package d01tut1;

import java.util.ArrayList;
import java.util.List;

class FinancialCalculator {
	List<Employee> employees;

	public FinancialCalculator() {
		employees = new ArrayList<Employee>();
	}

	public void addEmployee(Employee e) {
		employees.add(e);
	}

	public double getMonthlyPayExpense() {
		double pay = 0;

		for (Employee employee : employees) {
			pay += employee.getMonthlyPayExpense();
		}
		
		return pay;
	}
}