//#define _XBOX
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using OpenJigWare;

// Receive
using System.Threading;

namespace OjwRov
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
        }

        // 변수선언
        private Ojw.C3d m_C3d = new Ojw.C3d();              // 3D 모델링 시뮬레이션을 위한 변수 선언
        private Ojw.CSocket m_CSocket = new Ojw.CSocket();  // 소켓 제어를 위한 변수 선언

        private Ojw.CJoystick m_CJoy = new Ojw.CJoystick(Ojw.CJoystick._ID_0); // 조이스틱 선언
        private Ojw.CTimer m_CTmr_Joystick = new Ojw.CTimer(); // 조이스틱의 연결을 주기적으로 체크할 타이머

        private Ojw.CStream m_CStream = new Ojw.CStream(); // 스트리밍을 하기위한 변수
        /////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        private void Form1_Load(object sender, EventArgs e)
        {
            Ojw.CMessage.Init(txtMessage);
            // 기왕이면 파일 저장도... 실행 폴더에 보면 날짜별로 히스토리 파일 생성될 것임
            Ojw.CMessage.Init_File(true);
            
            // 3d 사용 준비 - 초기화(어디에 그릴 것인지 지정)
            m_C3d.Init(lbDisp);

            // 속성창 정의
            m_C3d.CreateProp_VirtualObject(lbProp);

            // 3D 모델링 오픈
            if (m_C3d.FileOpen("Oroca_Rov.ojw") == true) tmrJoystick.Enabled = true;
            else
            {
                Ojw.CMessage.Write_Error("모델링 파일 오픈 실패. 파일이 있는지 확인 하시오");
                Application.Exit();
            }

            // 타이머 초기화 - 이게 돌면서 실제 그림을 그려준다.
            m_CTmr_Throat.Set();
        }

        #region 소켓제어 - 이걸 펼치면 소켓 제어 부분 소스가 있다.
        #region Receive(TCP/IP)
        // Tcp/Ip Receive 를 위한 Thread - 아직 뭘 할지 계획 없다. 데이타 받아서 화면에 뿌리기만...
        private Thread m_thReceive;
        private void Thread_Receive()
        {
            Ojw.CMessage.Write("[Thread] is Started");
            while (m_CSocket.IsConnect() == true)
            {
                try
                {
                    byte data = m_CSocket.GetByte();
                    Ojw.CMessage.Write2("{0}", (char)data);
                }
                catch
                {
                    Ojw.CMessage.Write("[Thread] Port Closed");
                }
            }
            Ojw.CMessage.Write("[Thread] Closed Thread");
        }
        #endregion Receive(TCP/IP)

        private void btnConnect_Click(object sender, EventArgs e)
        {
            bool bClose = false;

            // 열려 있다면 닫고 닫혀있다면 열어주자.
            if (m_CSocket.IsConnect() == false)
            {
                // 소켓 열자
                m_CSocket.Connect(txtIp.Text, Ojw.CConvert.StrToInt(txtPort.Text));
            }
            else
            {
                bClose = true;
                // 이미 열려 있다면 닫아주는 걸로
                m_CSocket.DisConnect();
            }

            // 잘 열렸다면... 메세지를 출력하고 쓰레드를 동작시킨다.
            if (m_CSocket.IsConnect() == true)
            {
                Ojw.CMessage.Write("{0} is Opened(Port = {1}", txtIp.Text, txtPort.Text);
                btnConnect.Text = "DisConnect";
                
                // 쓰레드를 동작시킨다.
                m_thReceive = new Thread(new ThreadStart(Thread_Receive));
                m_thReceive.Start();
            }
            else // 접속 실패 시 메세지
            {
                btnConnect.Text = "Connect";
                if (bClose)
                    Ojw.CMessage.Write("Disconnected {0}(TCP)", txtIp.Text);
                else
                    Ojw.CMessage.Write_Error("We couldn\'t Open {0} [{1}]", txtIp.Text, txtPort.Text);
            }
        }

        // 데이타를 소켓으로 쏘는 부분
        private void SendValue(params SCommand_t[] pSCmd)
        {
            int nSize_Header = 5;
            int nSize_Data = pSCmd.Length * 3;
            byte[] pBuffer = new byte[nSize_Header + nSize_Data];
            byte[] pbyteTmp;
            int i = 0;
            int nCmd = 0x01;

            pBuffer[i++] = (byte)0x02;
            pBuffer[i++] = (byte)(nCmd & 0xff);
            byte byCheckSum = pBuffer[i - 1];
            pBuffer[i++] = (byte)(nSize_Data & 0xff);
            byCheckSum ^= pBuffer[i - 1];
            foreach (SCommand_t SCmd in pSCmd)
            {
                pBuffer[i++] = SCmd.byEn;
                byCheckSum ^= pBuffer[i - 1];
                pbyteTmp = Ojw.CConvert.ShortToBytes((short)SCmd.nValue);
                pBuffer[i++] = pbyteTmp[0];
                byCheckSum ^= pBuffer[i - 1];
                pBuffer[i++] = pbyteTmp[1];
                byCheckSum ^= pBuffer[i - 1];
            }
            pBuffer[i++] = byCheckSum; // Checksum
            pBuffer[i++] = (byte)0x03;
            m_CSocket.Send(pBuffer);
        }
        private void SendValue(params int[] pnData)
        {
            if (pnData.Length % 2 != 0)
            {
                Ojw.CMessage.Write_Error("Data Size error");
                return;
            }
            int nSize_Header = 5;
            int nSize_Data = pnData.Length / 2 * 3;
            byte[] pBuffer = new byte[nSize_Header + nSize_Data];
            byte[] pbyteTmp;
            int i = 0;
            int nCmd = 0x01;

            pBuffer[i++] = (byte)0x02;
            pBuffer[i++] = (byte)(nCmd & 0xff);
            byte byCheckSum = pBuffer[i - 1];
            pBuffer[i++] = (byte)(nSize_Data & 0xff);
            byCheckSum ^= pBuffer[i - 1];
            for (int nPos = 0; nPos < pnData.Length / 2; nPos++)
            {
                pBuffer[i++] = (byte)(pnData[nPos * 2] & 0xff);
                byCheckSum ^= pBuffer[i - 1];
                pbyteTmp = Ojw.CConvert.ShortToBytes((short)pnData[nPos * 2 + 1]);
                pBuffer[i++] = pbyteTmp[0];
                byCheckSum ^= pBuffer[i - 1];
                pBuffer[i++] = pbyteTmp[1];
                byCheckSum ^= pBuffer[i - 1];
            }
            pBuffer[i++] = byCheckSum; // Checksum
            pBuffer[i++] = (byte)0x03;
            m_CSocket.Send(pBuffer);
        }
        #endregion 소켓제어 - 이걸 펼치면 소켓 제어 부분 소스가 있다.

        private void tmrJoystick_Tick(object sender, EventArgs e)
        {
            // 조이스틱 정보 갱신
            m_CJoy.Update();

            // 조이스틱이 살아 있는지 체크하는 함수 - 중국 저가 RF 모델은 전원여부와 상관없이 상시 살아있다고 나타남.
            FJoystick_Check_Alive();

            // 조이스틱 데이타 체크
            FJoystick_Check_Data();

            // 3D 모델링을 그린다.
            m_C3d.OjwDraw();
        }

        private void FJoystick_Check_Alive()
        {
            #region Joystick Check

            Color m_colorLive = Color.LightGreen; // 살았을 경우의 색
            Color m_colorDead = Color.Gray; // 죽었을 경우의 색
            if (m_CJoy.IsValid == false)
            {
                #region 조이스틱이 연결되지 않았음을 표시
                if (lbJoystick.ForeColor != m_colorDead)
                {
                    lbJoystick.Text = "Joystick (No Connected)";
                    lbJoystick.ForeColor = m_colorDead;
                }
                #endregion 조이스틱이 연결되지 않았음을 표시

                #region 3초마다 다시 재연결을 하려고 시도
                if (m_CTmr_Joystick.Get() > 3000) // Joystic 이 죽어있다면 체크(3초단위)
                {
                    Ojw.CMessage.Write("Joystick Check again");
                    m_CJoy = new Ojw.CJoystick(Ojw.CJoystick._ID_0);

                    if (m_CJoy.IsValid == false)
                    {
                        Ojw.CMessage.Write("But we can't find a joystick device in here. Check your joystick device");
                        m_CTmr_Joystick.Set(); // 타이머의 카운터를 다시 초기화 한다.
                    }
                    else Ojw.CMessage.Write("Joystick is Connected");
                }
                #endregion 3초마다 다시 재연결을 하려고 시도
            }
            else
            {
                #region 연결 되었음을 표시
                if (lbJoystick.ForeColor != m_colorLive)
                {
                    lbJoystick.Text = "Joystick (Connected)";
                    lbJoystick.ForeColor = m_colorLive;
                }
                #endregion 연결 되었음을 표시
            }
            #endregion Joystick Check
        }

        private struct SCommand_t
        {
            public byte byEn;
            public int nValue;
        }
        
        private Ojw.CTimer m_CTmr_Throat = new Ojw.CTimer();
        private void FJoystick_Check_Data()
        {
            #region JoyStick
            // [ 아래는 XBox 기준이다. ]
            // 스틱좌상단, 0+ - 좌, 1+ - 하
            // 패드 2+ - 좌, 3+ - 하
            // 스틱우하단, 4+ - 좌, 5+ - 하
            // 버튼
            // Top 6 - -3 : 클릭
            // Left 7 - -3 : 클릭
            // Right 8 - -3 : 클릭
            // Bottom 9 - -3 : 클릭
            // 전면우측 10 - -3 : 클릭
            // 전면좌측 11 - -3 : 클릭
            // 전면우측아래 12- : 클릭
            // 전면좌측아래 13- : 클릭

            if (m_CJoy.IsValid == true)
            {
                //// 좌상단 조이스틱
                if (Math.Abs(m_CJoy.dX0 - 0.5) > 0.2)
                {
                    if (m_CTmr_Throat.Get() > 100)
                    {
                        double dData = (double)Math.Abs(m_CJoy.dX0 - 0.5);
                        if (dData > 0)
                        {
                            double dValue = (dData * 2) * 100.0;
                            int nValue2 = (int)((dData * 2) * 65535);
                            Ojw.CMessage.Write("Data = {0}", dValue);
                            SendValue(1, nValue2, 1, nValue2, 1, (int)dValue, 1, (int)dValue, 1, (int)dValue);
                        }
                        else
                        {
                            //SendValue(0, 0, 0, 0, 1, 0, 1, 0, 1, 0);
                        }
                        m_CTmr_Throat.Set();
                    }

                    m_C3d.SetData(1, (float)((m_CJoy.dX0 - 0.5) / 5.0 * 10.0));
                    m_C3d.SetData(2, (float)((m_CJoy.dX0 - 0.5) / 5.0 * 10.0));
                }
                else
                {
                    if (m_CTmr_Throat.Get() > 100)
                    {
                        m_CTmr_Throat.Set();
                    }
                    m_C3d.SetData(1, 0);
                    m_C3d.SetData(2, 0);
                }
                if (Math.Abs(m_CJoy.dY1 - 0.5) > 0.2) m_C3d.SetData(0, (float)10.0f);//((m_CJoy.dY0 - 0.5) / 5.0 * 10.0));
                else m_C3d.SetData(0, 0);
#if _XBOX //
                int nValue = 65535;
                if (m_CJoy.IsDown_Event(Ojw.CJoystick.PadKey.Button5) == true)
                //if (m_CJoy.IsDown_Event(Ojw.CJoystick.PadKey.Button7) == true)
                {
                    //m_CSocket.Send(Ojw.CConvert.StrToBytes("0"));
                    m_C3d.SetColor_Second(0, 4, 4, 0, Color.Cyan);
                    //nRight = 100;
                    //Ojw.CMessage.Write("Clicked 9"); 
                    Ojw.CMessage.Write("Light Right 100");
                    SendValue(1, nValue, 1, nValue, 0, 0, 0, 0, 0, 0);
                }
                else if (m_CJoy.IsUp_Event(Ojw.CJoystick.PadKey.Button5) == true)
                {
                    //nRight = 0;
                    m_C3d.SetColor_Second(0, 4, 4, 0, Color.Black);
                    Ojw.CMessage.Write("Light Right off");
                    SendValue(1, 0, 1, 0, 0, 0, 0, 0, 0, 0);
                }
                if (m_CJoy.IsDown_Event(Ojw.CJoystick.PadKey.Button6) == true)
                //if (m_CJoy.IsDown_Event(Ojw.CJoystick.PadKey.Button8) == true)
                {
                    //nLeft = 100;
                    m_C3d.SetColor_Second(1, 5, 5, 0, Color.Cyan);
                    //m_CSocket.Send(Ojw.CConvert.StrToBytes("1"));
                    Ojw.CMessage.Write("Light Left 100");
                    //Ojw.CMessage.Write("Clicked 10");
                    SendValue(1, nValue, 1, nValue, 0, 0, 0, 0, 0, 0);
                }
                else if (m_CJoy.IsUp_Event(Ojw.CJoystick.PadKey.Button6) == true)
                {
                    //nLeft = 0;
                    m_C3d.SetColor_Second(1, 5, 5, 0, Color.Black);
                    Ojw.CMessage.Write("Light Left off");
                    SendValue(1, 0, 1, 0, 0, 0, 0, 0, 0, 0);
                }
#else
                if (m_CJoy.IsDown_Event(Ojw.CJoystick.PadKey.Button7) == true)
                {
                    m_C3d.SetColor_Second(0, 4, 4, 0, Color.Cyan);
                    Ojw.CMessage.Write("Clicked 9");
                    SendValue(1, 100, 0, 0, 0, 0, 0, 0);
                }
                else if (m_CJoy.IsUp_Event(Ojw.CJoystick.PadKey.Button7) == true)
                {
                    m_C3d.SetColor_Second(0, 4, 4, 0, Color.Black);
                    SendValue(1, 0, 0, 0, 0, 0, 0, 0);
                }
                if (m_CJoy.IsDown_Event(Ojw.CJoystick.PadKey.Button8) == true)
                {
                    m_C3d.SetColor_Second(1, 5, 5, 0, Color.Cyan);
                    Ojw.CMessage.Write("Clicked 10");
                    SendValue(0, 0, 1, 100, 0, 0, 0, 0);
                }
                else if (m_CJoy.IsUp_Event(Ojw.CJoystick.PadKey.Button8) == true)
                {
                    m_C3d.SetColor_Second(1, 5, 5, 0, Color.Black);
                    SendValue(0, 0, 1, 0, 0, 0, 0, 0);
                }
#endif
                // 이 부분이 어떤 버튼이 눌리는지를 화면상에 뿌려준다.
                for (int i = 0; i < 30; i++)
                {
                    if (m_CJoy.IsDown_Event((Ojw.CJoystick.PadKey)i) == true) Ojw.CMessage.Write("Button Down{0}", i);
                    if (m_CJoy.IsUp_Event((Ojw.CJoystick.PadKey)i) == true) Ojw.CMessage.Write("Button Up{0}", i);
                }
            }
            #endregion JoyStick
        }
        
        private void btnStartStreaming_Click(object sender, EventArgs e)
        {
            //string strUrl = String.Format("http://{0}:{1}/?action=stream", txtIp.Text, txtPort_Cam.Text);// "http://192.168.20.6:8081";
            if (rdStream_Jpeg.Checked == true)
            {
                String strIp = txtIp.Text;
                int nPort = Ojw.CConvert.StrToInt(txtPort_Cam.Text);
                int nX = 0;
                int nY = 0;
                int nWidth = Ojw.CConvert.StrToInt(txtWidth.Text);
                int nHeight = Ojw.CConvert.StrToInt(txtHeight.Text);
                m_CStream.Start_Jpeg(pnDisp, strIp, nPort, nX, nY, nWidth, nHeight);
            }
            else
            {
                String strIp = txtIp.Text;
                int nPort = Ojw.CConvert.StrToInt(txtPort_Cam.Text);
                int nX = 0;
                int nY = 0;
                int nWidth = Ojw.CConvert.StrToInt(txtWidth.Text);
                int nHeight = Ojw.CConvert.StrToInt(txtHeight.Text);
                m_CStream.Start_MJpeg(pnDisp, strIp, nPort, nX, nY, nWidth, nHeight);
            }
        }
        
        private void btnStopStreaming_Click(object sender, EventArgs e)
        {
            m_CStream.Stop();
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_CStream.IsStreaming == true) m_CStream.Stop();
        }
    }
}
