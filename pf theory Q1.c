#include <stdio.h>
float calculateRepayment(float loan, float interestRate, int year, int totalYears, float installment) {
    if (loan <= 0 || year > totalYears) {
        printf("Loan fully repaid!\n");
        return 0;
    }
    loan = loan + (loan * interestRate);
    float newLoan = loan - installment;
    if (newLoan < 0)
        newLoan = 0;  
    printf("Year %d: Remaining loan = %.2f\n", year, newLoan);
    return installment + calculateRepayment(newLoan, interestRate, year + 1, totalYears, installment);
}
int main() {
    float loan = 100000;       
    float interestRate = 0.05; 
    int years = 3;             
    float installment = 40000; 
    printf("Loan Repayment Schedule:\n");
    float totalPaid = calculateRepayment(loan, interestRate, 1, years, installment);
    printf("\nTotal Repayment = %.2f\n", totalPaid);
    return 0;
}

