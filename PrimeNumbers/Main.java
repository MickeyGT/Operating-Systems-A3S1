public class Main
{
	public static void main(String[] args)
	{
		serialEratosthenes serial = new serialEratosthenes(10000000);
		serial.execute();

		for(int i=2;i<=8;i++)
		{
			parallelEratosthenes paralel = new parallelEratosthenes(i,10000000);
			paralel.execute();
		}
	}
}
