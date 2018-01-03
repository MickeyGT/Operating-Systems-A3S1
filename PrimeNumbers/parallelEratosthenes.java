import java.util.Arrays;
import java.util.concurrent.Semaphore;

public class parallelEratosthenes
{
	long start,end;
	int maxNumber;

	int nrThreads;
	Semaphore semaphore;
	public parallelEratosthenes(int nrT,int maxNr)
	{
		nrThreads=nrT;
		maxNumber=maxNr;
	}

	public void execute()
	{
		semaphore = new Semaphore(nrThreads,true);
		start = System.nanoTime();
		doEratosthenes();
		end = System.nanoTime();
		System.out.println("Parallel with "+nrThreads+" took "+(end-start));
	}

	private void doEratosthenes()
	{
		parallelThread.fill(maxNumber);
		int maxPrime = (int)Math.sqrt(maxNumber);
		for(int i=2;i<=maxPrime;i++)
			if(parallelThread.check(i)==true)
			{
				try
				{
					//System.out.print(i+" ");
					semaphore.acquire();
					parallelThread pthread = new parallelThread(i);
					pthread.run();
					semaphore.release();
				}
				catch (InterruptedException e)
				{
					e.printStackTrace();
				}
			}
		parallelThread.count();
	}
}
