import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.ArrayList;

class UserInterface extends JFrame {
	VisualSimulator vs;
	JTextArea LogTA;
	JPanel contentPane;
	JTextField fileTF;
	JTextField programNameTF;
	JTextField opStartAddrTF;
	JTextField lengthTF;
	JTextField decATF;
	JTextField hexATF;
	JTextField decXTF;
	JTextField hexXTF;
	JTextField decLTF;
	JTextField hexLTF;
	JTextField decPCTF;
	JTextField hexPCTF;
	JTextField hexSWTF;
	JTextField decBTF;
	JTextField hexBTF;
	JTextField decSTF;
	JTextField hexSTF;
	JTextField decTTF;
	JTextField hexTTF;
	JTextField hexFTF;
	JTextField firstInstTF;
	JTextField memStartAddrTF;
	JTextField taTF;
	JTextField deviceTF;

	JButton openBtn;
	JButton oneStepBtn;
	JButton allStepBtn;
	JButton exitBtn;

	JList<String> instList;

	/**
	 * Create the frame.
	 */
	public UserInterface(VisualSimulator _vs) {
		vs = _vs;
		setTitle("20163340 강원경");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 534, 670);

		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);

		JSeparator separator = new JSeparator();
		separator.setOrientation(SwingConstants.VERTICAL);
		separator.setBounds(250, 40, 11, 400);
		contentPane.add(separator);

		JLabel lblNewLabel = new JLabel("FileName:");
		lblNewLabel.setFont(new Font("Arial", Font.PLAIN, 12));
		lblNewLabel.setBounds(12, 10, 63, 15);
		contentPane.add(lblNewLabel);

		fileTF = new JTextField();
		fileTF.setBounds(87, 7, 154, 21);
		contentPane.add(fileTF);
		fileTF.setColumns(10);

		openBtn = new JButton("Open");
		openBtn.addActionListener(new ActionHandler());
		openBtn.setFont(new Font("Arial", Font.PLAIN, 12));
		openBtn.setBounds(249, 7, 73, 21);
		contentPane.add(openBtn);

		JLabel lblH = new JLabel("H (Header Record)");
		lblH.setFont(new Font("Arial", Font.PLAIN, 12));
		lblH.setBounds(22, 38, 118, 15);
		contentPane.add(lblH);

		JLabel lblProgramName = new JLabel("Program Name:");
		lblProgramName.setFont(new Font("Arial", Font.PLAIN, 12));
		lblProgramName.setBounds(32, 69, 97, 15);
		contentPane.add(lblProgramName);

		JLabel lblStartAddressOf = new JLabel("Start Address of");
		lblStartAddressOf.setFont(new Font("Arial", Font.PLAIN, 12));
		lblStartAddressOf.setBounds(31, 97, 98, 15);
		contentPane.add(lblStartAddressOf);

		JLabel lblOb = new JLabel("Object Program:");
		lblOb.setFont(new Font("Arial", Font.PLAIN, 12));
		lblOb.setBounds(31, 117, 98, 15);
		contentPane.add(lblOb);

		JLabel lblLeng = new JLabel("Length of Program:");
		lblLeng.setFont(new Font("Arial", Font.PLAIN, 12));
		lblLeng.setBounds(32, 142, 108, 15);
		contentPane.add(lblLeng);

		programNameTF = new JTextField();
		programNameTF.setBounds(125, 66, 116, 21);
		programNameTF.setEnabled(false);
		contentPane.add(programNameTF);
		programNameTF.setColumns(10);

		opStartAddrTF = new JTextField();
		opStartAddrTF.setBounds(137, 103, 104, 21);
		opStartAddrTF.setEnabled(false);
		contentPane.add(opStartAddrTF);
		opStartAddrTF.setColumns(10);

		lengthTF = new JTextField();
		lengthTF.setBounds(152, 139, 89, 21);
		lengthTF.setEnabled(false);
		contentPane.add(lengthTF);
		lengthTF.setColumns(10);

		JLabel label = new JLabel("Register");
		label.setFont(new Font("Arial", Font.PLAIN, 12));
		label.setBounds(12, 179, 57, 15);
		contentPane.add(label);

		JLabel lblDec = new JLabel("Dec");
		lblDec.setFont(new Font("Arial", Font.PLAIN, 12));
		lblDec.setBounds(83, 196, 57, 15);
		contentPane.add(lblDec);

		JLabel lblHex = new JLabel("Hex");
		lblHex.setFont(new Font("Arial", Font.PLAIN, 12));
		lblHex.setBounds(150, 196, 57, 15);
		contentPane.add(lblHex);

		JLabel lblA = new JLabel("A(#0)");
		lblA.setFont(new Font("Arial", Font.PLAIN, 12));
		lblA.setBounds(32, 221, 57, 15);
		contentPane.add(lblA);

		decATF = new JTextField();
		decATF.setBounds(80, 215, 64, 21);
		decATF.setEnabled(false);
		contentPane.add(decATF);
		decATF.setColumns(10);

		hexATF = new JTextField();
		hexATF.setBounds(150, 215, 73, 21);
		hexATF.setEnabled(false);
		contentPane.add(hexATF);
		hexATF.setColumns(10);

		JLabel lblX = new JLabel("X(#1)");
		lblX.setFont(new Font("Arial", Font.PLAIN, 12));
		lblX.setBounds(32, 246, 57, 15);
		contentPane.add(lblX);

		decXTF = new JTextField();
		decXTF.setColumns(10);
		decXTF.setEnabled(false);
		decXTF.setBounds(80, 240, 64, 21);
		contentPane.add(decXTF);

		hexXTF = new JTextField();
		hexXTF.setColumns(10);
		hexXTF.setEnabled(false);
		hexXTF.setBounds(150, 240, 73, 21);
		contentPane.add(hexXTF);

		JLabel lblL = new JLabel("L(#2)");
		lblL.setFont(new Font("Arial", Font.PLAIN, 12));
		lblL.setBounds(32, 271, 57, 15);
		contentPane.add(lblL);

		decLTF = new JTextField();
		decLTF.setColumns(10);
		decLTF.setEnabled(false);
		decLTF.setBounds(80, 265, 64, 21);
		contentPane.add(decLTF);

		hexLTF = new JTextField();
		hexLTF.setColumns(10);
		hexLTF.setEnabled(false);
		hexLTF.setBounds(150, 265, 73, 21);
		contentPane.add(hexLTF);

		JLabel lblB = new JLabel("B(#3)");
		lblB.setFont(new Font("Arial", Font.PLAIN, 12));
		lblB.setBounds(32, 296, 57, 15);
		contentPane.add(lblB);

		decBTF = new JTextField();
		decBTF.setColumns(10);
		decBTF.setEnabled(false);
		decBTF.setBounds(80, 290, 64, 21);
		contentPane.add(decBTF);

		hexBTF = new JTextField();
		hexBTF.setColumns(10);
		hexBTF.setEnabled(false);
		hexBTF.setBounds(150, 290, 73, 21);
		contentPane.add(hexBTF);

		JLabel lblS = new JLabel("S(#4)");
		lblS.setFont(new Font("Arial", Font.PLAIN, 12));
		lblS.setBounds(32, 321, 57, 15);
		contentPane.add(lblS);

		decSTF = new JTextField();
		decSTF.setColumns(10);
		decSTF.setEnabled(false);
		decSTF.setBounds(80, 315, 64, 21);
		contentPane.add(decSTF);

		hexSTF = new JTextField();
		hexSTF.setColumns(10);
		hexSTF.setEnabled(false);
		hexSTF.setBounds(150, 315, 73, 21);
		contentPane.add(hexSTF);

		JLabel lblT = new JLabel("T(#5)");
		lblT.setFont(new Font("Arial", Font.PLAIN, 12));
		lblT.setBounds(32, 346, 57, 15);
		contentPane.add(lblT);

		decTTF = new JTextField();
		decTTF.setColumns(10);
		decTTF.setEnabled(false);
		decTTF.setBounds(80, 340, 64, 21);
		contentPane.add(decTTF);

		hexTTF = new JTextField();
		hexTTF.setColumns(10);
		hexTTF.setEnabled(false);
		hexTTF.setBounds(150, 340, 73, 21);
		contentPane.add(hexTTF);

		JLabel lblF = new JLabel("F(#6)");
		lblF.setFont(new Font("Arial", Font.PLAIN, 12));
		lblF.setBounds(32, 371, 57, 15);
		contentPane.add(lblF);

		hexFTF = new JTextField();
		hexFTF.setColumns(10);
		hexFTF.setEnabled(false);
		hexFTF.setBounds(80, 365, 143, 21);
		contentPane.add(hexFTF);

		JLabel lblPC = new JLabel("PC(#8)");
		lblPC.setFont(new Font("Arial", Font.PLAIN, 12));
		lblPC.setBounds(32, 396, 57, 15);
		contentPane.add(lblPC);

		decPCTF = new JTextField();
		decPCTF.setColumns(10);
		decPCTF.setEnabled(false);
		decPCTF.setBounds(80, 390, 64, 21);
		contentPane.add(decPCTF);

		hexPCTF = new JTextField();
		hexPCTF.setColumns(10);
		hexPCTF.setEnabled(false);
		hexPCTF.setBounds(150, 390, 73, 21);
		contentPane.add(hexPCTF);

		JLabel lblSW = new JLabel("SW(#9)");
		lblSW.setFont(new Font("Arial", Font.PLAIN, 12));
		lblSW.setBounds(32, 421, 57, 15);
		contentPane.add(lblSW);

		hexSWTF = new JTextField();
		hexSWTF.setColumns(10);
		hexSWTF.setEnabled(false);
		hexSWTF.setBounds(80, 415, 143, 21);
		contentPane.add(hexSWTF);

		JLabel lblEndRecord = new JLabel("E(End Record)");
		lblEndRecord.setFont(new Font("Arial", Font.PLAIN, 12));
		lblEndRecord.setBounds(259, 38, 98, 15);
		contentPane.add(lblEndRecord);

		JLabel lblFA = new JLabel("Address of First Instruction");
		lblFA.setFont(new Font("Arial", Font.PLAIN, 12));
		lblFA.setBounds(269, 57, 159, 15);
		contentPane.add(lblFA);

		JLabel lblInObjectProgram = new JLabel("in Object Program:");
		lblInObjectProgram.setFont(new Font("Arial", Font.PLAIN, 12));
		lblInObjectProgram.setBounds(269, 75, 138, 15);
		contentPane.add(lblInObjectProgram);

		firstInstTF = new JTextField();
		firstInstTF.setBounds(390, 72, 73, 21);
		firstInstTF.setEnabled(false);
		contentPane.add(firstInstTF);
		firstInstTF.setColumns(10);

		JLabel lblStartAddressIn = new JLabel("Start Address in Memory");
		lblStartAddressIn.setFont(new Font("Arial", Font.PLAIN, 12));
		lblStartAddressIn.setBounds(260, 117, 184, 15);
		contentPane.add(lblStartAddressIn);

		memStartAddrTF = new JTextField();
		memStartAddrTF.setBounds(370, 136, 116, 21);
		memStartAddrTF.setEnabled(false);
		contentPane.add(memStartAddrTF);
		memStartAddrTF.setColumns(10);

		JLabel lblTargetAddress = new JLabel("Target Address:");
		lblTargetAddress.setFont(new Font("Arial", Font.PLAIN, 12));
		lblTargetAddress.setBounds(260, 162, 118, 15);
		contentPane.add(lblTargetAddress);

		taTF = new JTextField();
		taTF.setBounds(370, 159, 116, 21);
		taTF.setEnabled(false);
		contentPane.add(taTF);
		taTF.setColumns(10);

		JLabel lblInstructions = new JLabel("Instructions:");
		lblInstructions.setFont(new Font("Arial", Font.PLAIN, 12));
		lblInstructions.setBounds(266, 196, 73, 15);
		contentPane.add(lblInstructions);

		instList = new JList<String>();
		JScrollPane scrollPane2 = new JScrollPane();
		instList.setBounds(263, 220, 129, 216);
		scrollPane2.add(instList);
		scrollPane2.setBounds(263, 220, 129, 216);
		scrollPane2.setViewportView(instList);
		scrollPane2.setVerticalScrollBarPolicy(scrollPane2.VERTICAL_SCROLLBAR_ALWAYS);
		contentPane.add(scrollPane2);

		deviceTF = new JTextField();
		deviceTF.setBounds(440, 243, 63, 21);
		deviceTF.setEnabled(false);
		contentPane.add(deviceTF);
		deviceTF.setColumns(10);

		JLabel label_1 = new JLabel("사용중인 장치");
		label_1.setFont(new Font("맑은 고딕", Font.PLAIN, 12));
		label_1.setBounds(429, 221, 89, 15);
		contentPane.add(label_1);

		oneStepBtn = new JButton("실행(1 Step)");
		oneStepBtn.setFont(new Font("맑은 고딕", Font.PLAIN, 12));
		oneStepBtn.addActionListener(new ActionHandler());
		oneStepBtn.setBounds(404, 300, 108, 23);
		contentPane.add(oneStepBtn);

		allStepBtn = new JButton("실행  (All)");
		allStepBtn.setFont(new Font("맑은 고딕", Font.PLAIN, 12));
		allStepBtn.setBounds(404, 342, 108, 23);
		allStepBtn.addActionListener(new ActionHandler());
		contentPane.add(allStepBtn);

		exitBtn = new JButton("종료");
		exitBtn.setFont(new Font("맑은 고딕", Font.PLAIN, 12));
		exitBtn.setBounds(404, 382, 108, 23);
		exitBtn.addActionListener(new ActionHandler());
		contentPane.add(exitBtn);

		JLabel lblLog = new JLabel("Log(명령어 수행 관련):");
		lblLog.setFont(new Font("맑은 고딕", Font.PLAIN, 12));
		lblLog.setBounds(18, 460, 143, 15);
		contentPane.add(lblLog);

		JScrollPane scrollPane = new JScrollPane();
		LogTA = new JTextArea();
		LogTA.setBounds(22, 485, 484, 130);
		scrollPane.add(LogTA);
		scrollPane.setBounds(12, 485, 494, 130);
		scrollPane.setViewportView(LogTA);
		scrollPane.setVerticalScrollBarPolicy(scrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		contentPane.add(scrollPane);
	}

	private class ActionHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			JButton b = (JButton) e.getSource();
			if (b == openBtn) {
				JFileChooser fc = new JFileChooser(".");
				FileNameExtensionFilter filter = new FileNameExtensionFilter("Object File (*.obj)", "obj");
				fc.setFileFilter(filter);
				int choice = fc.showOpenDialog(null);
				if (choice == JFileChooser.APPROVE_OPTION) {
					File objFile = fc.getSelectedFile();
					fileTF.setText(objFile.getName());
					vs.load(objFile);
				}
			} else if (b == oneStepBtn) {
				vs.oneStep();
				instList.setSelectedIndex(instList.getModel().getSize() - 1);
				instList.ensureIndexIsVisible(instList.getSelectedIndex());
			} else if (b == allStepBtn) {
				vs.allStep();
				instList.setSelectedIndex(instList.getModel().getSize() - 1);
				instList.ensureIndexIsVisible(instList.getSelectedIndex());
				System.out.println(instList.getModel().getSize());
			} else if (b == exitBtn) {
				System.exit(0);
			}
		}
	}
}
