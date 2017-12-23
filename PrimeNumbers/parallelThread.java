import java.util.Arrays;

public class parallelThread implements Runnable
{
	int startNr;
	static int maxNumber;
	static Boolean[] primeStatus = new Boolean[10000100];

	static boolean check(int i)
	{
		return primeStatus[i];
	}

	static void fill(int maxNr)
	{
		maxNumber=maxNr;
		Arrays.fill(primeStatus, Boolean.TRUE);
	}

	parallelThread(int nr)
	{
		startNr=nr;
	}

	static void count()
	{
		int nrOfPrimes=0;
		for(int i=2;i<=maxNumber;i++)
			if(primeStatus[i]==true)
				nrOfPrimes++;
		System.out.println("There are "+nrOfPrimes+" prime numbers until "+maxNumber);
	}
	@Override
	public void run()
	{
		for(int j=startNr*2;j<=maxNumber;j+=startNr)
			primeStatus[j]=false;
	}
}
