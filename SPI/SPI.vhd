----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    09:46:02 04/01/2014 
-- Design Name: 
-- Module Name:    SPI - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity SPI is
	Generic (DataWidth : Integer := 8);
	
	Port( MClk : in  STD_LOGIC;
			Clk : in  STD_LOGIC;
			SS : in  STD_LOGIC;
         MISO : out  STD_LOGIC := '0';
         MOSI : in  STD_LOGIC;
			DataOut : out  STD_LOGIC_VECTOR(DataWidth-1 downto 0);
			DataIn : in  STD_LOGIC_VECTOR(DataWidth-1 downto 0);
			SignalPackageEnd : out  STD_LOGIC
			);
end SPI;

architecture Behavioral of SPI is
begin

-- recieves information through SPI. A SS pulse is requires inbetween all datapackages. When a Package is recieved the SignalPackageEnd goes high to signal a datapackages has been recieved/send and nextdata should be supplied on the ports/new data taken on rising edge.
PROCESS (Clk)
	variable DataBuffer : STD_LOGIC_VECTOR(DataWidth-1 downto 0);
	Variable pSS : STD_LOGIC := '1';
	variable pMClk : STD_LOGIC := '1';
BEGIN 
	if rising_edge(Clk) then 
		if SS = '0' AND pSS = '1' then
			-- SS falling edge --
			DataBuffer := DataIn;
			SignalPackageEnd <= '0';
		elsif SS = '0' AND pSS = '0' then
			-- SS = 0 --
			-- test for edges on MClk
			if MClk = '0' AND pMClk = '1' then
				-- rising edge --
				-- recieve data bit --
				DataBuffer := DataBuffer(DataWidth-2 downto 0) & MOSI;
			elsif MClk = '1' AND pMClk = '0' then
				-- falling edge --
				-- send databit --
				MISO <= DataBuffer(DataWidth-1);
			end if;
		elsif SS = '1' AND pSS = '0' then
			-- SS rising edge --
			-- put data out --
			DataOut <= DataBuffer;
			-- signal new data in buffer --
			SignalPackageEnd <= '1';
		else 
			-- SS = 1 --
		end if;
	end if;
END PROCESS;

end Behavioral;
