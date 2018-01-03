import java.util.Arrays;

public class serialEratosthenes
{
	long start,end;
	int maxNumber,nrOfPrimes;
	Boolean[] primeStatus = new Boolean[10000100];

	serialEratosthenes(int maxNr)
	{
		maxNumber=maxNr;
	}

	public void execute()
	{
		start = System.nanoTime();
		doEratosthenes();
		end = System.nanoTime();
		System.out.println("Serial took "+(end-start));
	}

	private void doEratosthenes()
	{
		nrOfPrimes=0;
		Arrays.fill(primeStatus, Boolean.TRUE);
		int maxPrime = (int)Math.sqrt(maxNumber);
		for(int i=2;i<=maxPrime;i++)
			if(primeStatus[i]==true)
			{
				for(int j=i*2;j<=maxNumber;j+=i)
					primeStatus[j]=false;
			}
		for(int i=2;i<=maxNumber;i++)
			if(primeStatus[i]==true)
				nrOfPrimes++;
		System.out.println("There are "+nrOfPrimes+" prime numbers until "+maxNumber);
	}
}
