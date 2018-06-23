using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using SLibrary_Warper;
using System.IO;
using System.Collections;
using System.Net;
using System.Configuration;

namespace Sin_MakeFramework
{
    class BTZPacket
    {
        public int packet_size;
        public UInt32 packet_id;
    }

    public enum ResultLevel : short
    {
		IMsg = 0,
        IErr = 1,
        ICri = 2,
        INull = 3,
    };

    public partial class UPeer
    {
        private Peer m_peer;

        public delegate void PacketProcess(MemoryStream ms);
        public delegate void ErrorHandle(string msg, short level);
        //    SLibrary_Warper.Peer.ErrorHandle errorHandle;

        public PacketProcess packetProcess;
        public ErrorHandle errorProcess;

        public void Connect(string ip, int port)
        {
            if (m_peer == null) m_peer = new Peer();

            m_peer.m_ErrorHandle = (SLibrary_Warper.ResultInfo info) => {
                errorProcess(info.msg, info.m_Level);
            };


            if (Marshal.SizeOf(new BTZPacket()) != NetworkDefine.pkt_size)
            {
                ResultInfo info = new ResultInfo("Not equal packetsize");
                m_peer.m_ErrorHandle(info);
                m_peer.m_ErrorHandle = null;
                return;
            }

            m_peer.m_PacketProcess = (List<char> data, int size) => {
                IntPtr ptr = Marshal.AllocHGlobal(size);
            
                MemoryStream ms = new MemoryStream(Encoding.Default.GetBytes(data.ToArray()));
                //or 
                //MemoryStream ms2 = new MemoryStream(Convert.FromBase64CharArray(data.ToArray(), 0, data.Count));

                packetProcess(ms);
            };
       

            unsafe
            {
                byte[] b2 = Encoding.GetEncoding(51949).GetBytes(ip);
                sbyte[] sb = (sbyte[])((Array)b2);
            

                fixed (sbyte* s_ip = sb)
                {
                    m_peer.Connect(s_ip, port);
                }
            }
        }

        public void Disconnect()
        {
           if(m_peer != null)
                m_peer.Disconnect();
        }

        public static T CastToPacket<T>(MemoryStream ms)
        {
            IntPtr ptr = Marshal.AllocHGlobal((int)ms.Length);

            try
            {
                T _t;

                _t = (T)Marshal.PtrToStructure(ptr, typeof(T));

                return _t;
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public void Recv()
        {
            m_peer.Recv();
        }

        public void Send<T>(T data)
        {
            MemoryStream ms = new MemoryStream();

            IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(data));

            try
            {
                Marshal.StructureToPtr(data, ptr, false);

                byte[] bytes = (byte[])Marshal.PtrToStructure(ptr, typeof(byte[]));

                ms.Write(bytes, 0, Marshal.SizeOf(data));

                m_peer.Send(ms);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
                ms.Close();
            }
        }
    }

}
